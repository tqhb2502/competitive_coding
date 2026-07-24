#!/usr/bin/env python3
"""Validator nhẹ cho dòng ``Tags:`` trong ``cses/**/idea.txt`` (không cần catalog).

Từ vựng hợp lệ = {tên file ``.hpp`` trong ``cp_library/<nhóm>/``} hợp với các dòng
trong ``tools/tag_vocab.txt``. Mỗi bài đã gắn tag phải có >= 1 tag là id snippet
(kỹ thuật cốt lõi). Chỉ kiểm format/vị trí/từ vựng, không kiểm ngữ nghĩa.
"""
import re
import sys
from pathlib import Path

# Console Windows mặc định cp1252 không mã hoá được tiếng Việt -> ép UTF-8 để không vỡ.
for _stream in (sys.stdout, sys.stderr):
    try:
        _stream.reconfigure(encoding="utf-8")
    except (AttributeError, ValueError):
        pass

TAG_LINE = re.compile(r"^\s*tags\s*:", re.IGNORECASE)
URL_LINE = re.compile(r"^https://cses\.fi/problemset/task/[0-9]+/?$")
TAG_TOKEN = re.compile(r"^[a-z0-9]+(?:-[a-z0-9]+)*$")
PREFIX = "Tags: "


def load_vocabulary(lib_root):
    ids = {hpp.stem for hpp in lib_root.glob("[0-9][0-9]_*/*.hpp")}
    modifiers = set()
    vocab_file = lib_root / "tools" / "tag_vocab.txt"
    if vocab_file.is_file():
        for line in vocab_file.read_text(encoding="utf-8").splitlines():
            line = line.strip()
            if line and not line.startswith("#"):
                modifiers.add(line)
    return ids, ids | modifiers


def validate(text, ids, allowed):
    lines = text.split("\n")
    tag_lines = [i for i, ln in enumerate(lines) if TAG_LINE.match(ln)]
    if not tag_lines:
        return []  # chưa gắn tag vẫn hợp lệ
    if len(tag_lines) > 1:
        return ["có nhiều hơn một dòng Tags"]
    idx = tag_lines[0]
    line = lines[idx]
    if not line.startswith(PREFIX):
        return ["dòng tag phải bắt đầu đúng bằng {!r}".format(PREFIX)]
    urls = [i for i, ln in enumerate(lines) if URL_LINE.match(ln)]
    if len(urls) != 1:
        return ["cần đúng một dòng URL cses"]
    u = urls[0]
    placed = (idx == u + 2 and lines[u + 1] == "" and idx + 2 < len(lines)
              and lines[idx + 1] == "" and lines[idx + 2].startswith("## "))
    if not placed:
        return ["dòng Tags sai vị trí (đúng: URL, trống, Tags, trống, dòng '## ')"]
    tags = line[len(PREFIX):].split(", ")
    errs = []
    if not tags or any(t == "" for t in tags):
        return ["danh sách tag rỗng hoặc lỗi phân tách"]
    if line != PREFIX + ", ".join(sorted(set(tags))):
        errs.append("tag phải tăng dần theo từ điển, không trùng, phân tách bằng ', '")
    for t in tags:
        if not TAG_TOKEN.match(t):
            errs.append("tag không hợp lệ: {!r}".format(t))
        elif t not in allowed:
            errs.append("tag ngoài từ vựng: {!r}".format(t))
    if not any(t in ids for t in tags):
        errs.append("cần >= 1 tag là id snippet (kỹ thuật cốt lõi)")
    return errs


def main():
    lib_root = Path(__file__).resolve().parents[1]
    repo_root = lib_root.parent
    cses_root = repo_root / "cses"
    if not cses_root.is_dir():
        print("error: không thấy thư mục cses/", file=sys.stderr)
        return 2
    ids, allowed = load_vocabulary(lib_root)
    total = tagged = assignments = 0
    ok = True
    for idea in sorted(cses_root.rglob("idea.txt")):
        total += 1
        text = idea.read_text(encoding="utf-8")
        errs = validate(text, ids, allowed)
        tag_line = next((ln for ln in text.split("\n") if ln.startswith(PREFIX)), None)
        if tag_line is not None and not errs:
            tagged += 1
            assignments += len(tag_line[len(PREFIX):].split(", "))
        for e in errs:
            ok = False
            print("error: {}: {}".format(idea.relative_to(repo_root).as_posix(), e),
                  file=sys.stderr)
    if not ok:
        return 2
    print("CSES tags hợp lệ: {} idea, {} đã gắn, {} lượt tag "
          "(chỉ kiểm format/vị trí/từ vựng).".format(total, tagged, assignments))
    return 0


if __name__ == "__main__":
    sys.exit(main())
