#!/usr/bin/env python3
"""Sinh dòng ``// CSES: <mã bài>`` trong mỗi snippet .hpp từ ``Tags:`` của cses/**/idea.txt.

Nguồn chân lý là các dòng ``Tags:`` (link xuôi bài -> kỹ thuật). Tool này tạo link
ngược: với mỗi id kỹ thuật (tên file .hpp), liệt kê mã số các bài CSES dùng nó.
Chạy ``--check`` để chỉ kiểm (fail nếu chưa cập nhật), không ghi.
"""
import re
import sys
from pathlib import Path

for _s in (sys.stdout, sys.stderr):
    try:
        _s.reconfigure(encoding="utf-8")
    except (AttributeError, ValueError):
        pass

PREFIX = "// CSES:"
TAGS = "Tags: "


def build_tag_map(cses_root):
    tag_to_ids = {}
    for idea in cses_root.rglob("idea.txt"):
        task = idea.parent.name.split("_", 1)[0]
        if not task.isdigit():
            continue
        for line in idea.read_text(encoding="utf-8").split("\n"):
            if line.startswith(TAGS):
                for tag in line[len(TAGS):].split(", "):
                    tag_to_ids.setdefault(tag, set()).add(int(task))
                break
    return tag_to_ids


def desired_line(ids):
    return PREFIX + " " + (" ".join(str(x) for x in sorted(ids)) if ids else "—")


def apply(hpp, line, check):
    text = hpp.read_text(encoding="utf-8")
    rows = text.split("\n")
    for i, r in enumerate(rows):
        if r.startswith(PREFIX):
            if r == line:
                return "unchanged"
            if check:
                return "stale"
            rows[i] = line
            with open(hpp, "w", encoding="utf-8", newline="") as f:
                f.write("\n".join(rows))
            return "updated"
    # chưa có: chèn sau khối comment header ngay dưới 'using namespace std;'
    u = next((i for i, r in enumerate(rows) if r.startswith("using namespace std;")), None)
    if u is None:
        return "skip-no-using"
    j = u + 1
    while j < len(rows) and rows[j].lstrip().startswith("//"):
        j += 1
    if check:
        return "stale"
    rows[j:j] = [line]
    with open(hpp, "w", encoding="utf-8", newline="") as f:
        f.write("\n".join(rows))
    return "inserted"


def main():
    check = "--check" in sys.argv[1:]
    lib_root = Path(__file__).resolve().parents[1]
    cses_root = lib_root.parent / "cses"
    tag_map = build_tag_map(cses_root) if cses_root.is_dir() else {}
    stats = {}
    stale = []
    for hpp in sorted(lib_root.glob("[0-9][0-9]_*/*.hpp")):
        line = desired_line(tag_map.get(hpp.stem, set()))
        action = apply(hpp, line, check)
        stats[action] = stats.get(action, 0) + 1
        if action == "stale":
            stale.append(hpp.stem)
    if check and stale:
        print("error: dòng // CSES chưa cập nhật ở: " + ", ".join(stale)
              + " — chạy `python3 tools/link_cses.py`", file=sys.stderr)
        return 1
    print("link_cses:", stats)
    return 0


if __name__ == "__main__":
    sys.exit(main())
