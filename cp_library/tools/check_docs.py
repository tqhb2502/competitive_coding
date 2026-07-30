#!/usr/bin/env python3
"""Kiểm tính NHẤT QUÁN giữa tài liệu và hệ thống tệp của cp_library.

Chạy: python3 cp_library/tools/check_docs.py
Trả về mã lỗi khác 0 nếu có lỗi (dùng trong tools/verify.sh).

Kiểm:
1. Cấu trúc mỗi snippet .hpp: #pragma once, bits/stdc++.h, using namespace std,
   header 5 dòng (mô tả + Khi dùng/ĐPT/Dùng/Bẫy), dòng '// CSES:', demo CP_DEMO,
   và có khối doc hàm ('// Ý nghĩa:' ...).
2. README.md liệt kê ĐÚNG MỘT lần mọi snippet (không thiếu, không thừa).
3. Mọi liên kết markdown nội bộ trong .md đều trỏ tới tệp/thư mục có thật.
4. indexes/ phủ hết snippet: by-use-case.md và decision-trees.md phải nhắc mọi id.
"""
import re
import sys
from pathlib import Path

for _s in (sys.stdout, sys.stderr):
    _s.reconfigure(encoding="utf-8")

LIB = Path(__file__).resolve().parent.parent
GROUP_RE = re.compile(r"^\d\d_")
LINK_RE = re.compile(r"\[[^\]]*\]\(([^)#]+)(?:#[^)]*)?\)")

errors: list[str] = []
warns: list[str] = []


def snippets() -> list[Path]:
    out = []
    for d in sorted(LIB.iterdir()):
        if d.is_dir() and GROUP_RE.match(d.name):
            out += sorted(d.glob("*.hpp"))
    return out


def check_structure(path: Path) -> None:
    rel = path.relative_to(LIB).as_posix()
    text = path.read_text(encoding="utf-8")
    lines = text.split("\n")
    if not lines or lines[0].strip() != "#pragma once":
        errors.append(f"{rel}: dòng đầu phải là '#pragma once'")
    if "#include <bits/stdc++.h>" not in text:
        errors.append(f"{rel}: thiếu '#include <bits/stdc++.h>'")
    if "using namespace std;" not in text:
        errors.append(f"{rel}: thiếu 'using namespace std;'")
    header = "\n".join(lines[:12])
    # Nhãn cho phép hậu tố trong ngoặc, ví dụ 'Dùng (mẫu longest):'.
    for label, pat in (("Khi dùng", r"^//\s*Khi dùng"), ("ĐPT", r"^//\s*ĐPT"),
                       ("Dùng", r"^//\s*Dùng"), ("Bẫy", r"^//\s*Bẫy")):
        if not re.search(pat, header, re.M):
            errors.append(f"{rel}: header thiếu nhãn '{label}:' (phải nằm trong 12 dòng đầu)")
    cses = [l for l in lines if l.startswith("// CSES:")]
    if len(cses) != 1:
        errors.append(f"{rel}: phải có ĐÚNG MỘT dòng '// CSES:' (đang có {len(cses)})")
    if "#ifdef CP_DEMO" not in text or "#endif" not in text:
        errors.append(f"{rel}: thiếu khối demo '#ifdef CP_DEMO ... #endif'")
    elif "int main(" not in text:
        errors.append(f"{rel}: khối CP_DEMO thiếu 'int main()'")
    # Phần thân = mọi thứ NGOÀI khối demo (main trần ở đây sẽ phá bundle).
    body = re.sub(r"#ifdef CP_DEMO.*?#endif", "", text, flags=re.S)
    if re.search(r"^\s*int main\s*\(", body, re.M):
        errors.append(f"{rel}: có 'int main' ngoài khối CP_DEMO (sẽ phá bundle)")
    n_y = text.count("// Ý nghĩa:")
    # Khối nguyên hàm dùng chung của hình học (guard CP_GEO_BASE) chỉ chú thích inline -> miễn.
    body_no_geo = re.sub(r"#ifndef CP_GEO_BASE.*?#endif", "", body, flags=re.S)
    has_func = re.search(r"\)\s*(?:const\s*)?\{", body_no_geo) is not None
    if n_y == 0 and has_func:
        errors.append(f"{rel}: không có khối doc hàm nào ('// Ý nghĩa:')")
    for label in ("// Tham số:", "// Trả về:"):
        if text.count(label) > n_y:
            errors.append(f"{rel}: số dòng '{label}' nhiều hơn số '// Ý nghĩa:' (doc lệch khối)")


def check_readme_coverage(ids: dict[str, Path]) -> None:
    readme = (LIB / "README.md").read_text(encoding="utf-8")
    listed: dict[str, int] = {}
    for m in LINK_RE.finditer(readme):
        target = m.group(1).strip()
        if target.endswith(".hpp"):
            listed[target] = listed.get(target, 0) + 1
    for rel in sorted(ids):
        if rel not in listed:
            errors.append(f"README.md: THIẾU snippet {rel} trong bảng tra cứu")
        elif listed[rel] > 1:
            errors.append(f"README.md: snippet {rel} xuất hiện {listed[rel]} lần (phải đúng một)")
    for rel in sorted(listed):
        if rel not in ids:
            errors.append(f"README.md: trỏ tới snippet KHÔNG tồn tại {rel}")


def strip_code(text: str) -> str:
    """Bỏ code (khối ``` và inline `...`) để lambda C++ như [&](int u) không bị nhận là liên kết."""
    text = re.sub(r"```.*?```", "", text, flags=re.S)
    return re.sub(r"`[^`]*`", "", text)


def check_links() -> None:
    for md in sorted(LIB.rglob("*.md")):
        base = md.parent
        for m in LINK_RE.finditer(strip_code(md.read_text(encoding="utf-8"))):
            target = m.group(1).strip()
            if target.startswith(("http://", "https://", "mailto:")):
                continue
            if not (base / target).exists():
                errors.append(f"{md.relative_to(LIB).as_posix()}: liên kết CHẾT -> {target}")


def check_index_coverage(ids: dict[str, Path]) -> None:
    for name in ("by-use-case.md", "decision-trees.md"):
        path = LIB / "indexes" / name
        if not path.is_file():
            errors.append(f"indexes/{name}: không tồn tại")
            continue
        text = path.read_text(encoding="utf-8")
        missing = []
        for rel in sorted(ids):
            snippet_id = rel.split("/")[-1][:-4]
            exact_id = re.compile(
                rf"(?<![a-z0-9-]){re.escape(snippet_id)}(?![a-z0-9-])"
            )
            if exact_id.search(text) is None:
                missing.append(snippet_id)
        if missing:
            errors.append(f"indexes/{name}: chưa nhắc {len(missing)} snippet: {', '.join(missing)}")


def check_geo_guard(files: list) -> None:
    """Khối #ifndef CP_GEO_BASE bị copy vào từng file hình học: mọi bản phải GIỐNG HỆT.

    Nếu một bản lệch, kết quả bundle phụ thuộc THỨ TỰ include (file đầu tiên thắng) -> sai âm thầm.
    """
    blocks = {}
    for p in files:
        if p.parent.name != "08_geometry":
            continue
        m = re.search(r"#ifndef CP_GEO_BASE.*?#endif", p.read_text(encoding="utf-8"), re.S)
        rel = p.relative_to(LIB).as_posix()
        if not m:
            errors.append(f"{rel}: file hình học thiếu khối guard '#ifndef CP_GEO_BASE ... #endif'")
            continue
        # Bỏ comment cuối dòng: chỉ ĐỊNH NGHĨA phải giống nhau, chú thích được phép khác.
        code = "\n".join(
            line.split("//")[0].rstrip() for line in m.group(0).split("\n")
        ).strip()
        blocks.setdefault(code, []).append(rel)
    if len(blocks) > 1:
        groups = " | ".join(f"[{', '.join(v)}]" for v in blocks.values())
        errors.append(f"08_geometry: khối CP_GEO_BASE KHÔNG giống nhau giữa các file: {groups}")


def main() -> int:
    files = snippets()
    if not files:
        print("check_docs: KHÔNG tìm thấy snippet nào?", file=sys.stderr)
        return 1
    ids = {p.relative_to(LIB).as_posix(): p for p in files}
    for p in files:
        check_structure(p)
    check_geo_guard(files)
    check_readme_coverage(ids)
    check_links()
    check_index_coverage(ids)

    for w in warns:
        print(f"  cảnh báo: {w}")
    if errors:
        print(f"check_docs: {len(errors)} LỖI tài liệu:", file=sys.stderr)
        for e in errors:
            print(f"  - {e}", file=sys.stderr)
        return 1
    print(f"Tài liệu nhất quán: {len(files)} snippet, README + indexes phủ đủ, không có liên kết chết.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
