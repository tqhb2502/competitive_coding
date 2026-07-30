#!/usr/bin/env python3
"""Sinh dòng ``// CSES: <mã bài>`` trong mỗi snippet .hpp từ ``Tags:`` của cses/**/idea.txt.

Nguồn chân lý là các dòng ``Tags:`` (link xuôi bài -> kỹ thuật). Tool này tạo link
ngược: với mỗi id kỹ thuật (tên file .hpp), liệt kê mã số các bài CSES dùng nó.
Chạy ``--check`` để chỉ kiểm (fail nếu chưa cập nhật), không ghi.
"""
import argparse
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
ALLOWLIST = "cses_unmapped_allowlist.txt"


def build_tag_map(cses_root):
    tag_to_ids = {}
    seen_tasks = {}
    for idea in sorted(cses_root.rglob("idea.txt")):
        raw_task = idea.parent.name.split("_", 1)[0]
        if not raw_task.isdigit():
            raise SystemExit(
                "{}: thư mục bài phải bắt đầu bằng id số".format(idea)
            )
        task = int(raw_task)
        if task in seen_tasks:
            raise SystemExit(
                "error: trùng CSES id {} giữa {} và {}".format(
                    task, seen_tasks[task], idea
                )
            )
        seen_tasks[task] = idea
        tag_lines = [
            line for line in idea.read_text(encoding="utf-8").split("\n")
            if line.startswith(TAGS)
        ]
        if len(tag_lines) != 1:
            raise SystemExit(
                "{}: cần đúng một dòng {!r}, đang có {}".format(
                    idea, TAGS, len(tag_lines)
                )
            )
        tags = tag_lines[0][len(TAGS):].split(", ")
        if not tags or any(not tag for tag in tags):
            raise SystemExit("{}: danh sách Tags rỗng".format(idea))
        for tag in tags:
            tag_to_ids.setdefault(tag, set()).add(task)
    return tag_to_ids


def desired_line(ids):
    return PREFIX + " " + (" ".join(str(x) for x in sorted(ids)) if ids else "—")


def load_unmapped_allowlist(tools_root):
    path = tools_root / ALLOWLIST
    allowed = set()
    if not path.is_file():
        return allowed
    for number, raw in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
        stripped = raw.strip()
        if not stripped or stripped.startswith("#"):
            continue
        entry, separator, reason = raw.partition("#")
        line = entry.strip()
        if not re.fullmatch(r"[a-z0-9]+(?:-[a-z0-9]+)*", line):
            raise SystemExit("{}:{}: id không hợp lệ: {!r}".format(path, number, line))
        if not separator or not reason.strip():
            raise SystemExit(
                "{}:{}: ngoại lệ phải có lý do sau dấu #: {}".format(
                    path, number, line
                )
            )
        if line in allowed:
            raise SystemExit("{}:{}: id bị lặp: {}".format(path, number, line))
        allowed.add(line)
    return allowed


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
    # Chưa có: chèn NGAY SAU dòng 'Bẫy:' — dòng cuối của header 5 dòng (quy ước, check_docs bắt buộc).
    # Không dùng mốc 'using namespace std;' vì file có thêm using/include (vd ordered-set.hpp)
    # sẽ bị chèn chen giữa các dòng using.
    b = next((i for i, r in enumerate(rows) if re.match(r"^//\s*Bẫy", r)), None)
    if b is not None:
        j = b + 1
        while j < len(rows) and rows[j].lstrip().startswith("//") and not rows[j].startswith(PREFIX):
            j += 1   # header có thể dài hơn 5 dòng (Bẫy tràn dòng)
    else:
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


def parse_arguments(arguments=None):
    parser = argparse.ArgumentParser(
        description="Sinh hoặc kiểm dòng // CSES trong mọi snippet từ Tags của CSES."
    )
    parser.add_argument(
        "--check",
        action="store_true",
        help="chỉ kiểm drift, không ghi tệp",
    )
    return parser.parse_args(arguments)


def main(arguments=None):
    options = parse_arguments(arguments)
    check = options.check
    lib_root = Path(__file__).resolve().parents[1]
    cses_root = lib_root.parent / "cses"
    if not cses_root.is_dir():
        print("error: không thấy thư mục cses/ cạnh cp_library/", file=sys.stderr)
        return 2

    snippet_paths = sorted(lib_root.glob("[0-9][0-9]_*/*.hpp"))
    by_id = {}
    for hpp in snippet_paths:
        by_id.setdefault(hpp.stem, []).append(
            hpp.relative_to(lib_root).as_posix()
        )
    duplicates = {snippet_id: paths for snippet_id, paths in by_id.items()
                  if len(paths) > 1}
    if duplicates:
        for snippet_id, paths in sorted(duplicates.items()):
            print(
                "error: id snippet bị trùng {}: {}".format(
                    snippet_id, ", ".join(paths)
                ),
                file=sys.stderr,
            )
        return 2

    tag_map = build_tag_map(cses_root)
    allow_unmapped = load_unmapped_allowlist(Path(__file__).resolve().parent)
    snippet_ids = set(by_id)
    unknown_allowlist = sorted(allow_unmapped - snippet_ids)
    obsolete_allowlist = sorted(tag for tag in allow_unmapped if tag_map.get(tag))
    unexplained = sorted(tag for tag in snippet_ids if not tag_map.get(tag)
                         and tag not in allow_unmapped)
    metadata_errors = []
    if unknown_allowlist:
        metadata_errors.append("allowlist chứa snippet không tồn tại: " + ", ".join(unknown_allowlist))
    if obsolete_allowlist:
        metadata_errors.append("allowlist đã lỗi thời (snippet nay đã có CSES): "
                               + ", ".join(obsolete_allowlist))
    if unexplained:
        metadata_errors.append("snippet chưa có bài CSES và chưa được giải trình: "
                               + ", ".join(unexplained))
    if metadata_errors:
        for message in metadata_errors:
            print("error: " + message, file=sys.stderr)
        return 1

    stats = {}
    stale = []
    for hpp in snippet_paths:
        line = desired_line(tag_map.get(hpp.stem, set()))
        action = apply(hpp, line, check)
        stats[action] = stats.get(action, 0) + 1
        if action == "stale":
            stale.append(hpp.stem)
    if check and stale:
        print("error: dòng // CSES chưa cập nhật ở: " + ", ".join(stale)
              + " — chạy `python3 tools/link_cses.py`", file=sys.stderr)
        return 1
    mapped = len(snippet_ids) - len(allow_unmapped)
    print("link_cses:", stats, "coverage: {}/{} snippet có bài, {} ngoại lệ có lý do"
          .format(mapped, len(snippet_ids), len(allow_unmapped)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
