#!/usr/bin/env python3
"""Sinh phần TRA CỨU của tài liệu trực tiếp từ header các snippet .hpp.

Chạy:  python3 cp_library/tools/gen_docs.py           # ghi lại tài liệu
       python3 cp_library/tools/gen_docs.py --check   # chỉ kiểm lệch (dùng trong verify.sh)

Sinh:
- indexes/by-use-case.md   : bảng "Khi dùng" theo nhóm (giữa cặp mốc GEN)
- indexes/by-complexity.md : bảng độ phức tạp theo nhóm (giữa cặp mốc GEN)
- indexes/pitfalls.md      : danh sách mọi dòng "Bẫy" (giữa cặp mốc GEN)
- indexes/cses.md          : đủ 400 bài, tên bài, tag/snippet và link idea
- <nhóm>/README.md         : bảng snippet của từng nhóm (toàn bộ tệp)

Nhờ sinh tự động, tài liệu không bao giờ lệch với header thật; verify.sh chạy --check.
"""
import argparse
import re
import sys
from pathlib import Path

for _s in (sys.stdout, sys.stderr):
    _s.reconfigure(encoding="utf-8")

LIB = Path(__file__).resolve().parent.parent
REPO = LIB.parent
CSES = REPO / "cses"
GROUP_RE = re.compile(r"^\d\d_")
BEGIN, END = "<!-- GEN:BEGIN (tự sinh bởi tools/gen_docs.py — đừng sửa tay) -->", "<!-- GEN:END -->"
TITLE_RE = re.compile(r"^(.*?)\s+-\s+CSES\s+([0-9]+)\s*$")

GROUP_TITLES = {
    "01_foundations": "01 — Nền tảng (mảng, đoạn, hai con trỏ, bitmask)",
    "02_data_structures": "02 — Cấu trúc dữ liệu",
    "03_graphs": "03 — Đồ thị",
    "04_trees": "04 — Cây",
    "05_dynamic_programming": "05 — Quy hoạch động",
    "06_strings": "06 — Xâu",
    "07_mathematics": "07 — Toán",
    "08_geometry": "08 — Hình học",
    "09_offline_algorithms": "09 — Thuật toán offline",
    "10_advanced_techniques": "10 — Kỹ thuật nâng cao",
    "11_greedy": "11 — Greedy (tham lam trên đoạn / lịch biểu)",
}


def parse(path: Path) -> dict:
    """Bóc thông tin từ header 5 dòng + dòng CSES của một snippet."""
    # Một số header có Dùng/Bẫy xuống nhiều dòng; đọc rộng hơn để không làm rơi
    # dòng CSES (ví dụ NTT + CRT) nhưng vẫn chỉ lấy occurrence metadata đầu tiên.
    lines = path.read_text(encoding="utf-8").split("\n")[:40]
    info = {"id": path.stem, "group": path.parent.name, "file": f"{path.parent.name}/{path.name}"}
    for line in lines:
        if not line.startswith("//"):
            continue
        body = line[2:].strip()
        if body.startswith("Khi dùng:"):
            info.setdefault("when", body[len("Khi dùng:"):].strip())
        elif body.startswith("ĐPT:"):
            info.setdefault("cost", body[len("ĐPT:"):].strip())
        elif body.startswith("Bẫy:"):
            info.setdefault("trap", body[len("Bẫy:"):].strip())
        elif body.startswith("CSES:"):
            info.setdefault("cses", body[len("CSES:"):].strip())
        elif "—" in body and "name" not in info:
            name, _, purpose = body.partition("—")
            info["name"] = name.strip()
            info["purpose"] = purpose.strip().rstrip(".")
    for key, default in (("name", path.stem), ("purpose", ""), ("when", ""), ("cost", ""), ("trap", ""), ("cses", "—")):
        info.setdefault(key, default)
    return info


def snippets() -> dict[str, list[dict]]:
    out: dict[str, list[dict]] = {}
    locations: dict[str, list[str]] = {}
    for d in sorted(LIB.iterdir()):
        if d.is_dir() and GROUP_RE.match(d.name):
            paths = sorted(d.glob("*.hpp"))
            out[d.name] = [parse(p) for p in paths]
            for path in paths:
                locations.setdefault(path.stem, []).append(
                    path.relative_to(LIB).as_posix()
                )
    duplicates = {snippet_id: paths for snippet_id, paths in locations.items()
                  if len(paths) > 1}
    if duplicates:
        details = "; ".join(
            "{} -> {}".format(snippet_id, ", ".join(paths))
            for snippet_id, paths in sorted(duplicates.items())
        )
        raise SystemExit(
            "error: id snippet phải duy nhất trên toàn thư viện: " + details
        )
    return out


def cses_catalog() -> dict[int, dict]:
    """Đọc tên, đường dẫn và Tags của mọi bài từ cses/**/idea.txt.

    Dòng đầu ``<tên> - CSES <id>`` là nguồn tên hiển thị; URL và id thư mục
    vẫn được validator độc lập kiểm tra trong check_cses_tags.py.
    """
    out: dict[int, dict] = {}
    if not CSES.is_dir():
        raise SystemExit("error: không thấy thư mục cses/ cạnh cp_library/")
    for idea in sorted(CSES.rglob("idea.txt")):
        lines = idea.read_text(encoding="utf-8").split("\n")
        match = TITLE_RE.match(lines[0]) if lines else None
        if not match:
            raise SystemExit(
                f"{idea.relative_to(REPO).as_posix()}: dòng đầu phải có dạng "
                "'<tên> - CSES <id>'"
            )
        title, raw_id = match.groups()
        task = int(raw_id)
        if task in out:
            raise SystemExit(
                "error: trùng CSES id {} giữa {} và {}".format(
                    task, out[task]["idea"], idea.relative_to(REPO).as_posix()
                )
            )
        tags = None
        for line in lines:
            if line.startswith("Tags: "):
                tags = line[len("Tags: "):].split(", ")
                break
        if tags is None:
            raise SystemExit(
                "{}: thiếu dòng Tags".format(
                    idea.relative_to(REPO).as_posix()
                )
            )
        out[task] = {
            "id": task,
            "title": title,
            "tags": tags,
            "idea": idea.relative_to(REPO).as_posix(),
            "group": idea.parent.parent.name,
        }
    return out


def esc(text: str) -> str:
    """Escape ký tự markdown: '|' phá bảng, '[]' bị hiểu là liên kết tham chiếu (vd a[i])."""
    return text.replace("|", "\\|").replace("[", "\\[").replace("]", "\\]")


def cut(text: str, limit: int = 150) -> str:
    text = esc(text)
    return text if len(text) <= limit else text[: limit - 1].rstrip() + "…"


def block_use_case(data) -> str:
    parts = []
    for g, items in data.items():
        parts.append(f"### {GROUP_TITLES[g]}\n")
        parts.append("| Kỹ thuật | Khi nào dùng (dấu hiệu đề) | File |")
        parts.append("|---|---|---|")
        for it in items:
            parts.append(f"| `{it['id']}` | {cut(it['when'])} | [{it['id']}.hpp](../{it['file']}) |")
        parts.append("")
    return "\n".join(parts).rstrip()


def block_complexity(data) -> str:
    parts = []
    for g, items in data.items():
        parts.append(f"### {GROUP_TITLES[g]}\n")
        parts.append("| Kỹ thuật | Độ phức tạp & bộ nhớ | File |")
        parts.append("|---|---|---|")
        for it in items:
            parts.append(f"| `{it['id']}` | {cut(it['cost'], 170)} | [{it['id']}.hpp](../{it['file']}) |")
        parts.append("")
    return "\n".join(parts).rstrip()


def block_pitfalls(data) -> str:
    parts = []
    for g, items in data.items():
        parts.append(f"### {GROUP_TITLES[g]}\n")
        for it in items:
            parts.append(f"- [`{it['id']}`](../{it['file']}): {esc(it['trap'])}")
        parts.append("")
    return "\n".join(parts).rstrip()


def cses_cell(raw_ids: str, catalog: dict[int, dict]) -> str:
    """Đổi ``// CSES: 1 2`` thành link có cả mã và TÊN bài."""
    if raw_ids == "—":
        return "— *(không có bài trực tiếp trong bộ CSES hiện tại)*"
    links = []
    for token in raw_ids.split():
        if not token.isdigit() or int(token) not in catalog:
            links.append(esc(token))
            continue
        task = int(token)
        title = esc(catalog[task]["title"])
        links.append(f"[{task} {title}](https://cses.fi/problemset/task/{task})")
    return "<br>".join(links)


def group_readme(g: str, items: list[dict], catalog: dict[int, dict]) -> str:
    head = (
        f"# {GROUP_TITLES[g]}\n\n"
        f"{len(items)} snippet. Tệp này **tự sinh** bởi `tools/gen_docs.py` từ header các `.hpp` — đừng sửa tay.\n"
        "Tra theo dấu hiệu đề: [indexes/by-use-case.md](../indexes/by-use-case.md) · "
        "cây quyết định: [indexes/decision-trees.md](../indexes/decision-trees.md) · "
        "bẫy: [indexes/pitfalls.md](../indexes/pitfalls.md)\n\n"
        "| Kỹ thuật | Mục đích | Bài CSES | File |\n|---|---|---|---|\n"
    )
    rows = [
        f"| {it['name']} | {cut(it['purpose'], 110)} | {cses_cell(it['cses'], catalog)} | "
        f"[{it['id']}.hpp]({it['id']}.hpp) |"
        for it in items
    ]
    return head + "\n".join(rows) + "\n"


def cses_index(data: dict[str, list[dict]], catalog: dict[int, dict]) -> str:
    """Sinh cả hai bảng bài -> snippet và snippet -> tên bài trong một tệp."""
    by_id = {
        item["id"]: item["file"]
        for items in data.values()
        for item in items
    }
    tagged = sum(bool(item["tags"]) for item in catalog.values())
    assignments = sum(len(item["tags"]) for item in catalog.values())
    lines = [
        "# Đối chiếu hai chiều CSES ↔ CP Library",
        "",
        "Tệp này **tự sinh** bởi `tools/gen_docs.py`; nguồn chân lý là dòng `Tags:` "
        "trong từng `cses/**/idea.txt`.",
        f"Coverage hiện tại: **{tagged}/{len(catalog)} bài có tag**, "
        f"**{assignments} lượt tag**. Bảng tra ngược snippet → tên bài nằm cuối "
        "tệp này và cũng xuất hiện trong README của từng nhóm thư viện.",
        "",
        "| ID | Bài CSES | Nhóm | Tags / snippet | Idea |",
        "|---:|---|---|---|---|",
    ]
    for task, item in sorted(catalog.items()):
        rendered_tags = []
        for tag in item["tags"]:
            if tag in by_id:
                rendered_tags.append(f"[`{tag}`](../{by_id[tag]})")
            else:
                rendered_tags.append(f"`{tag}`")
        tags = ", ".join(rendered_tags) if rendered_tags else "—"
        title = esc(item["title"])
        idea_from_index = "../../" + item["idea"]
        lines.append(
            f"| {task} | [{title}](https://cses.fi/problemset/task/{task}) | "
            f"`{item['group']}` | {tags} | [idea.txt]({idea_from_index}) |"
        )
    tasks_by_snippet = {snippet_id: [] for snippet_id in by_id}
    for task, item in sorted(catalog.items()):
        for tag in item["tags"]:
            if tag in tasks_by_snippet:
                tasks_by_snippet[tag].append(task)
    lines += [
        "",
        "## Tra ngược: snippet → bài CSES",
        "",
        "Mỗi dòng gom toàn bộ bài có `Tags:` chứa đúng id snippet. Dấu `—` chỉ "
        "được phép với ngoại lệ đã giải trình trong "
        "[`tools/cses_unmapped_allowlist.txt`](../tools/cses_unmapped_allowlist.txt).",
        "",
        "| Snippet | Nhóm | Bài CSES áp dụng |",
        "|---|---|---|",
    ]
    for group, items in data.items():
        for item in items:
            task_ids = tasks_by_snippet[item["id"]]
            raw_ids = " ".join(str(task) for task in task_ids) if task_ids else "—"
            lines.append(
                f"| [`{item['id']}`](../{item['file']}) | `{group}` | "
                f"{cses_cell(raw_ids, catalog)} |"
            )
    return "\n".join(lines) + "\n"


def replace_block(path: Path, new_block: str) -> str:
    text = path.read_text(encoding="utf-8")
    if BEGIN not in text or END not in text:
        raise SystemExit(f"{path.relative_to(LIB).as_posix()}: thiếu cặp mốc GEN:BEGIN/GEN:END")
    head, _, rest = text.partition(BEGIN)
    _, _, tail = rest.partition(END)
    return f"{head}{BEGIN}\n\n{new_block}\n\n{END}{tail}"


def parse_arguments(arguments=None):
    parser = argparse.ArgumentParser(
        description="Sinh tài liệu tra cứu từ metadata snippet và Tags CSES."
    )
    parser.add_argument(
        "--check",
        action="store_true",
        help="chỉ kiểm drift, không ghi tệp",
    )
    return parser.parse_args(arguments)


def main(arguments=None) -> int:
    options = parse_arguments(arguments)
    check = options.check
    data = snippets()
    catalog = cses_catalog()
    total = sum(len(v) for v in data.values())
    targets: list[tuple[Path, str]] = [
        (LIB / "indexes" / "by-use-case.md", replace_block(LIB / "indexes" / "by-use-case.md", block_use_case(data))),
        (LIB / "indexes" / "by-complexity.md", replace_block(LIB / "indexes" / "by-complexity.md", block_complexity(data))),
        (LIB / "indexes" / "pitfalls.md", replace_block(LIB / "indexes" / "pitfalls.md", block_pitfalls(data))),
    ]
    for g, items in data.items():
        targets.append((LIB / g / "README.md", group_readme(g, items, catalog)))
    targets.append((LIB / "indexes" / "cses.md", cses_index(data, catalog)))

    drift = []
    for path, content in targets:
        old = path.read_text(encoding="utf-8") if path.is_file() else None
        if old == content:
            continue
        drift.append(path.relative_to(LIB).as_posix())
        if not check:
            with open(path, "w", encoding="utf-8", newline="\n") as fh:
                fh.write(content)
    if check:
        if drift:
            print(f"gen_docs --check: {len(drift)} tệp LỆCH với header snippet:", file=sys.stderr)
            for d in drift:
                print(f"  - {d}", file=sys.stderr)
            print("  chạy 'python3 cp_library/tools/gen_docs.py' để sinh lại.", file=sys.stderr)
            return 1
        print(f"Tài liệu tra cứu khớp header của {total} snippet.")
        return 0
    print(f"Đã sinh tài liệu tra cứu từ {total} snippet ({len(drift)} tệp cập nhật).")
    return 0


if __name__ == "__main__":
    sys.exit(main())
