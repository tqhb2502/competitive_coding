#!/usr/bin/env python3
"""Validator cho metadata và dòng ``Tags:`` trong ``cses/**/idea.txt``.

Từ vựng hợp lệ = {tên file ``.hpp`` trong ``cp_library/<nhóm>/``} hợp với các dòng
trong ``tools/tag_vocab.txt``. Mặc định MỌI bài phải có đúng một dòng tag và ít
nhất một tag phải là id snippet (kỹ thuật cốt lõi). Trong lúc migration có thể
dùng ``--allow-untagged``; CI/verify không dùng cờ này.
"""
import argparse
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
URL_LINE = re.compile(r"^https://cses\.fi/problemset/task/([0-9]+)/?$")
TITLE_LINE = re.compile(r"^(.+?)\s+-\s+CSES\s+([0-9]+)\s*$")
TAG_TOKEN = re.compile(r"^[a-z0-9]+(?:-[a-z0-9]+)*$")
PREFIX = "Tags: "


def load_vocabulary(lib_root):
    by_id = {}
    for hpp in sorted(lib_root.glob("[0-9][0-9]_*/*.hpp")):
        by_id.setdefault(hpp.stem, []).append(hpp.relative_to(lib_root).as_posix())
    duplicates = {snippet_id: paths for snippet_id, paths in by_id.items()
                  if len(paths) > 1}
    if duplicates:
        details = "; ".join(
            "{} -> {}".format(snippet_id, ", ".join(paths))
            for snippet_id, paths in sorted(duplicates.items())
        )
        raise SystemExit(
            "error: id snippet phải duy nhất trên toàn thư viện: " + details
        )
    ids = set(by_id)
    modifiers = set()
    vocab_file = lib_root / "tools" / "tag_vocab.txt"
    if vocab_file.is_file():
        for line in vocab_file.read_text(encoding="utf-8").splitlines():
            line = line.strip()
            if line and not line.startswith("#"):
                modifiers.add(line)
    return ids, ids | modifiers


def validate(text, ids, allowed, expected_task=None, allow_untagged=False):
    lines = text.split("\n")
    errs = []
    title = TITLE_LINE.match(lines[0]) if lines else None
    if not title:
        errs.append("dòng đầu phải có dạng '<tên bài> - CSES <id>'")
    elif expected_task is not None and int(title.group(2)) != expected_task:
        errs.append("id ở tiêu đề {} khác id thư mục {}".format(title.group(2), expected_task))

    urls = [(i, URL_LINE.match(ln)) for i, ln in enumerate(lines) if URL_LINE.match(ln)]
    if len(urls) != 1:
        errs.append("cần đúng một dòng URL cses")
    elif expected_task is not None and int(urls[0][1].group(1)) != expected_task:
        errs.append("id URL {} khác id thư mục {}".format(urls[0][1].group(1), expected_task))

    tag_lines = [i for i, ln in enumerate(lines) if TAG_LINE.match(ln)]
    if not tag_lines:
        if not allow_untagged:
            errs.append("thiếu dòng Tags")
        return errs
    if len(tag_lines) > 1:
        errs.append("có nhiều hơn một dòng Tags")
        return errs
    idx = tag_lines[0]
    line = lines[idx]
    if not line.startswith(PREFIX):
        errs.append("dòng tag phải bắt đầu đúng bằng {!r}".format(PREFIX))
        return errs
    if len(urls) != 1:
        return errs
    u = urls[0][0]
    placed = (idx == u + 2 and lines[u + 1] == "" and idx + 2 < len(lines)
              and lines[idx + 1] == "" and lines[idx + 2].startswith("## "))
    if not placed:
        errs.append("dòng Tags sai vị trí (đúng: URL, trống, Tags, trống, dòng '## ')")
    tags = line[len(PREFIX):].split(", ")
    if not tags or any(t == "" for t in tags):
        errs.append("danh sách tag rỗng hoặc lỗi phân tách")
        return errs
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


def parse_arguments(arguments=None):
    parser = argparse.ArgumentParser(
        description="Kiểm metadata và Tags trong mọi cses/**/idea.txt."
    )
    parser.add_argument(
        "--allow-untagged",
        action="store_true",
        help="chỉ dùng tạm trong migration; verify không bật cờ này",
    )
    return parser.parse_args(arguments)


def main(arguments=None):
    options = parse_arguments(arguments)
    allow_untagged = options.allow_untagged
    lib_root = Path(__file__).resolve().parents[1]
    repo_root = lib_root.parent
    cses_root = repo_root / "cses"
    if not cses_root.is_dir():
        print("error: không thấy thư mục cses/", file=sys.stderr)
        return 2
    ids, allowed = load_vocabulary(lib_root)
    total = tagged = assignments = 0
    seen_tasks = {}
    ok = True
    task_dirs = sorted(
        path for group in cses_root.iterdir() if group.is_dir()
        for path in group.iterdir()
        if path.is_dir() and path.name.split("_", 1)[0].isdigit()
    )
    task_dir_set = set(task_dirs)
    for task_dir in task_dirs:
        if not (task_dir / "idea.txt").is_file():
            ok = False
            print("error: {}: thiếu idea.txt".format(
                task_dir.relative_to(repo_root).as_posix()), file=sys.stderr)

    for idea in sorted(cses_root.rglob("idea.txt")):
        total += 1
        text = idea.read_text(encoding="utf-8")
        raw_task = idea.parent.name.split("_", 1)[0]
        expected_task = int(raw_task) if raw_task.isdigit() else None
        errs = validate(text, ids, allowed, expected_task, allow_untagged)
        if idea.parent not in task_dir_set:
            errs.append(
                "idea.txt phải nằm trực tiếp ở cses/<nhóm>/<id>_<slug>/idea.txt"
            )
        if not any((idea.parent / name).is_file()
                   for name in ("solution.cpp", "solution.py")):
            errs.append("thiếu cả solution.cpp lẫn solution.py cạnh idea.txt")
        if expected_task is None:
            errs.append("thư mục bài phải bắt đầu bằng id số")
        elif expected_task in seen_tasks:
            errs.append("trùng id với {}".format(seen_tasks[expected_task]))
        else:
            seen_tasks[expected_task] = idea.relative_to(repo_root).as_posix()
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
    mode = "cho phép bài chưa tag (migration)" if allow_untagged else "bắt buộc coverage đầy đủ"
    print("CSES tags hợp lệ: {} thư mục bài, {} idea, {} đã gắn, {} lượt tag; {}."
          .format(len(task_dirs), total, tagged, assignments, mode))
    return 0


if __name__ == "__main__":
    sys.exit(main())
