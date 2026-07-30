#!/usr/bin/env bash
# Kiểm chứng nhẹ cho kho snippet: mọi .hpp biên dịch độc lập, template chạy được,
# luồng bundle-1-file hoạt động, và dòng Tags CSES đúng format/từ vựng.
# YÊU CẦU: chạy bằng bash THẬT (git bash), không phải bash rút gọn của w64devkit.
#   export PATH="$HOME/Downloads/w64devkit/bin:$PATH"   # ưu tiên đúng g++
#   bash cp_library/tools/verify.sh
set -euo pipefail

tools_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
lib="$(cd "${tools_dir}/.." && pwd)"
repo="$(cd "${lib}/.." && pwd)"
cxx="${CXX:-g++}"
skip_gnu_only="${SKIP_GNU_ONLY:-0}"
if [ "${skip_gnu_only}" != "0" ] && [ "${skip_gnu_only}" != "1" ]; then
    printf 'error: SKIP_GNU_ONLY chỉ nhận 0 hoặc 1\n' >&2
    exit 1
fi
python_cmd=()
if [ -n "${PYTHON:-}" ]; then
    python_cmd=("${PYTHON}")
elif command -v python3 >/dev/null 2>&1 &&
        python3 -c 'import sys; raise SystemExit(sys.version_info < (3, 9))' \
        >/dev/null 2>&1; then
    python_cmd=(python3)
elif command -v python >/dev/null 2>&1 &&
        python -c 'import sys; raise SystemExit(sys.version_info < (3, 9))' \
        >/dev/null 2>&1; then
    python_cmd=(python)
elif command -v py >/dev/null 2>&1 &&
        py -3 -c 'import sys; raise SystemExit(sys.version_info < (3, 9))' \
        >/dev/null 2>&1; then
    # Python Launcher là cách cài phổ biến trên Windows và thường không tạo python3.exe.
    python_cmd=(py -3)
else
    printf 'error: cần Python >= 3.9 (python3, python, `py -3` hoặc biến PYTHON)\n' >&2
    exit 1
fi
if ! "${python_cmd[@]}" -c 'import sys; raise SystemExit(sys.version_info < (3, 9))'; then
    printf 'error: cp_library tools yêu cầu Python >= 3.9\n' >&2
    exit 1
fi
# FAST=1 dùng -O0 cho vòng lặp sửa-kiểm (nhanh hơn đáng kể); mặc định -O2 như lúc nộp bài.
if [ "${FAST:-0}" = "1" ]; then
    flags="-std=c++17 -O0 -Wall -Wextra"
    printf 'chế độ FAST=1: biên dịch -O0\n'
else
    flags="-std=c++17 -O2 -Wall -Wextra"
fi

# g++ (bản Windows) cần path kiểu "d:/..." chứ không hiểu MSYS "/d/...".
win() { cygpath -m "$1" 2>/dev/null || printf '%s' "$1"; }

tmp="$(mktemp -d "${TMPDIR:-/tmp}/cp-verify.XXXXXX")"
trap 'rm -rf "${tmp}"' EXIT
tmpw="$(win "${tmp}")"
repow="$(win "${repo}")"

# Danh sách snippet dùng chung cho compile độc lập và compile chung một TU.
# Chỉ dùng SKIP_GNU_ONLY=1 trên host không có GNU PBDS (ví dụ Apple Clang);
# verify chính thức bằng MinGW/GCC phải giữ mặc định 0.
snippet_files() {
    while IFS= read -r path; do
        if [ "${skip_gnu_only}" = "1" ] &&
                [ "$(basename "${path}")" = "ordered-set.hpp" ]; then
            continue
        fi
        printf '%s\n' "${path}"
    done < <(find "${lib}" -mindepth 2 -maxdepth 2 -name '*.hpp' \
        -path '*/[0-9][0-9]_*' | LC_ALL=C sort)
}
if [ "${skip_gnu_only}" = "1" ]; then
    printf 'cảnh báo: SKIP_GNU_ONLY=1, bỏ compile ordered-set.hpp (cần GNU PBDS)\n'
fi

# 1. Mỗi snippet .hpp phải tự chứa: include vào driver rỗng rồi biên dịch.
count=0
while IFS= read -r hpp; do
    printf '#include "%s"\nint main() { return 0; }\n' "$(win "${hpp}")" > "${tmp}/driver${count}.cpp"
    # shellcheck disable=SC2086
    "${cxx}" ${flags} "${tmpw}/driver${count}.cpp" -o "${tmpw}/a${count}.exe"
    # Demo #ifdef CP_DEMO: include qua driver để GCC không cảnh báo
    # "#pragma once in main file" (flag tắt cảnh báo đó của Clang không portable).
    # Tên output duy nhất theo counter để tránh race ghi đè .exe trên Windows.
    printf '#define CP_DEMO\n#include "%s"\n' "$(win "${hpp}")" \
        > "${tmp}/demo_driver${count}.cpp"
    # shellcheck disable=SC2086
    "${cxx}" ${flags} "${tmpw}/demo_driver${count}.cpp" \
        -o "${tmpw}/demo${count}.exe"
    # Demo phải IN RA gì đó: bắt trường hợp demo chạy nhưng im lặng (logic bị hỏng/ra rỗng).
    if ! demo_output="$("${tmpw}/demo${count}.exe" < /dev/null)"; then
        printf 'error: demo của %s thoát với mã lỗi\n' "${hpp}" >&2
        exit 1
    fi
    if [ -z "${demo_output}" ]; then
        printf 'error: demo của %s không in ra gì\n' "${hpp}" >&2
        exit 1
    fi
    count=$((count + 1))
done < <(snippet_files)
printf 'compiled + ran %d snippet header(s) và demo\n' "${count}"

# 1b. Mọi snippet phải cùng tồn tại trong một translation unit: bắt tên global trùng,
#     macro/guard rò rỉ và kết quả phụ thuộc thứ tự include.
: > "${tmp}/all_headers.cpp"
while IFS= read -r hpp; do
    printf '#include "%s"\n' "$(win "${hpp}")" >> "${tmp}/all_headers.cpp"
done < <(snippet_files)
printf 'int main() { return 0; }\n' >> "${tmp}/all_headers.cpp"
# shellcheck disable=SC2086
"${cxx}" ${flags} "${tmpw}/all_headers.cpp" -o "${tmpw}/all_headers.exe"
"${tmpw}/all_headers.exe"
printf 'all snippet headers coexist in one translation unit\n'

# 2. Template thi đấu phải biên dịch (có -I gốc repo vì template include snippet) và chạy an toàn không cần input.
tcount=0
for cpp in "${lib}"/templates/*.cpp; do
    # shellcheck disable=SC2086
    "${cxx}" ${flags} -I "${repow}" "$(win "${cpp}")" -o "${tmpw}/template${tcount}.exe"
    "${tmpw}/template${tcount}.exe" < /dev/null > /dev/null
    tcount=$((tcount + 1))
done
# debug.hpp: phải biên dịch được cả khi BẬT và khi TẮT CP_DEBUG (khi nộp là tắt).
printf '#define CP_DEMO\n#define CP_DEBUG\n#include "%s"\n' \
    "$(win "${lib}/templates/debug.hpp")" > "${tmp}/debug_on.cpp"
# shellcheck disable=SC2086
"${cxx}" ${flags} "${tmpw}/debug_on.cpp" -o "${tmpw}/debug_on.exe"
"${tmpw}/debug_on.exe" < /dev/null > /dev/null 2>&1
printf '#define CP_DEMO\n#include "%s"\n' \
    "$(win "${lib}/templates/debug.hpp")" > "${tmp}/debug_off.cpp"
# shellcheck disable=SC2086
"${cxx}" ${flags} "${tmpw}/debug_off.cpp" -o "${tmpw}/debug_off.exe"
"${tmpw}/debug_off.exe" < /dev/null > /dev/null
printf 'templates (%d) + debug.hpp compiled and ran\n' "${tcount}"

# 2b. Bộ khung stress test phải tự chạy được: brute vs brute qua gen -> luôn khớp.
stress_out="$(bash "${lib}/templates/stress.sh" "${lib}/templates/brute.cpp" "${lib}/templates/brute.cpp" \
    "${lib}/templates/gen.cpp" 3 2>&1 | tail -1)"
if [ "${stress_out}" != "Khớp toàn bộ 3 vòng." ]; then
    printf 'error: stress.sh smoke thất bại: %s\n' "${stress_out}" >&2
    exit 1
fi
printf 'stress.sh smoke passed\n'

# 2c. So output phải bỏ khác biệt whitespace giữa CÁC token nhưng không được
#     nuốt mất separator: "1 2" và "12" là hai dãy token khác nhau.
cat > "${tmp}/token_sol.cpp" <<'EOF'
#include <cstdio>
int main() { std::printf("1 2\n"); }
EOF
cat > "${tmp}/token_brute.cpp" <<'EOF'
#include <cstdio>
int main() { std::printf("12\n"); }
EOF
if bash "${lib}/templates/stress.sh" "${tmp}/token_sol.cpp" \
        "${tmp}/token_brute.cpp" "${lib}/templates/gen.cpp" 1 \
        > /dev/null 2>&1; then
    printf 'error: stress.sh coi output "1 2" và "12" là giống nhau\n' >&2
    exit 1
fi
printf 'stress.sh token-separator rejection passed\n'

# 3. Bundle smoke test: gộp 1 file rồi biên dịch/chạy, kiểm output cố định.
cat > "${tmp}/main.cpp" <<'EOF'
#include <bits/stdc++.h>
using namespace std;
#include "cp_library/02_data_structures/dsu.hpp"
#include "cp_library/02_data_structures/fenwick.hpp"
int main() {
    DSU d(5);
    d.unite(0, 1);
    Fenwick f(5);
    f.add(0, 3);
    printf("%d %lld\n", (int)d.same(0, 1), f.sum(5));
    return 0;
}
EOF
"${python_cmd[@]}" "$(win "${tools_dir}/bundle.py")" "${tmpw}/main.cpp" -I "${repow}" -o "${tmpw}/submit.cpp"
# shellcheck disable=SC2086
"${cxx}" ${flags} "${tmpw}/submit.cpp" -o "${tmpw}/submit.exe"
bundle_output="$("${tmpw}/submit.exe")"
if [ "${bundle_output}" != "1 3" ]; then
    printf 'error: bundle smoke output %q, expected "1 3"\n' "${bundle_output}" >&2
    exit 1
fi
printf 'bundle-to-one-file smoke passed\n'

# 3b. Bundle ca DỄ VỠ: nhiều file 08_geometry (khối guard CP_GEO_BASE lặp lại), include TRÙNG,
#     include LỒNG (template include snippet), và bundle ra STDOUT (đường in ra có ký tự tiếng Việt).
cat > "${tmp}/geo_main.cpp" <<'EOF'
#include <bits/stdc++.h>
using namespace std;
#include "cp_library/08_geometry/geometry-basics.hpp"
#include "cp_library/08_geometry/convex-hull.hpp"
#include "cp_library/08_geometry/point-in-polygon.hpp"
#include "cp_library/08_geometry/closest-pair.hpp"
#include "cp_library/08_geometry/convex-hull.hpp"
int main() {
    vector<Point> p = {{0, 0}, {4, 0}, {4, 4}, {0, 4}, {2, 2}};
    auto h = convexHull(p);
    printf("%d %d %lld\n", (int)h.size(), pointInPolygon(h, {2, 2}), closestPair(p));
    return 0;
}
EOF
"${python_cmd[@]}" "$(win "${tools_dir}/bundle.py")" "${tmpw}/geo_main.cpp" -I "${repow}" -o "${tmpw}/geo_submit.cpp"
# shellcheck disable=SC2086
"${cxx}" ${flags} "${tmpw}/geo_submit.cpp" -o "${tmpw}/geo_submit.exe"
geo_output="$("${tmpw}/geo_submit.exe")"
if [ "${geo_output}" != "4 2 8" ]; then
    printf 'error: bundle hình học output %q, expected "4 2 8"\n' "${geo_output}" >&2
    exit 1
fi
"${python_cmd[@]}" "$(win "${tools_dir}/bundle.py")" "${tmpw}/geo_main.cpp" -I "${repow}" > "${tmpw}/geo_stdout.cpp"
# shellcheck disable=SC2086
"${cxx}" ${flags} "${tmpw}/geo_stdout.cpp" -o "${tmpw}/geo_stdout.exe"
"${python_cmd[@]}" "$(win "${tools_dir}/bundle.py")" "$(win "${lib}/templates/geometry.cpp")" -I "${repow}" -o "${tmpw}/tpl_bundle.cpp"
# shellcheck disable=SC2086
"${cxx}" ${flags} "${tmpw}/tpl_bundle.cpp" -o "${tmpw}/tpl_bundle.exe"
"${tmpw}/tpl_bundle.exe" < /dev/null > /dev/null
printf 'bundle geometry/guard + include trùng + stdout + template passed\n'

# 3c. Đường dẫn quoted include tuyệt đối phải bị từ chối trên mọi host, kể cả
#     cú pháp drive Windows mà PurePosixPath không xem là absolute.
cat > "${tmp}/absolute_include.cpp" <<'EOF'
#include "C:/competitive_coding/cp_library/01_foundations/prefix-sum.hpp"
int main() { return 0; }
EOF
if "${python_cmd[@]}" "$(win "${tools_dir}/bundle.py")" \
        "${tmpw}/absolute_include.cpp" -I "${repow}" \
        -o "${tmpw}/absolute_submit.cpp" > /dev/null 2> "${tmp}/absolute_error.txt"; then
    printf 'error: bundle đã chấp nhận quoted include tuyệt đối kiểu Windows\n' >&2
    exit 1
fi
if ! grep -q 'absolute quoted include is not allowed' "${tmp}/absolute_error.txt"; then
    printf 'error: bundle từ chối path tuyệt đối nhưng thông báo sai:\n' >&2
    cat "${tmp}/absolute_error.txt" >&2
    exit 1
fi
printf 'bundle absolute-path rejection passed\n'

# 3d. Output là hardlink alias của source/header cũng phải bị từ chối. Chỉ
#     so Path.resolve() là chưa đủ vì hai tên khác nhau có thể chung inode/file ID.
cat > "${tmp}/hardlink_source.cpp" <<'EOF'
int main() { return 0; }  // MUST_SURVIVE_BUNDLE_CHECK
EOF
cp "${tmp}/hardlink_source.cpp" "${tmp}/hardlink_source.backup"
"${python_cmd[@]}" -c \
    'import os, sys; os.link(sys.argv[1], sys.argv[2])' \
    "${tmpw}/hardlink_source.cpp" "${tmpw}/hardlink_output.cpp"
if "${python_cmd[@]}" "$(win "${tools_dir}/bundle.py")" \
        "${tmpw}/hardlink_source.cpp" -o "${tmpw}/hardlink_output.cpp" \
        > /dev/null 2> "${tmp}/hardlink_error.txt"; then
    printf 'error: bundle đã ghi đè source qua một hardlink alias\n' >&2
    exit 1
fi
if ! grep -q 'output must not overwrite the source or an included file' \
        "${tmp}/hardlink_error.txt"; then
    printf 'error: bundle từ chối hardlink output nhưng thông báo sai:\n' >&2
    cat "${tmp}/hardlink_error.txt" >&2
    exit 1
fi
if ! cmp -s "${tmp}/hardlink_source.cpp" "${tmp}/hardlink_source.backup"; then
    printf 'error: source đã bị thay đổi khi kiểm tra hardlink output\n' >&2
    exit 1
fi
printf 'bundle hardlink-overwrite rejection passed\n'

# 4. Biên dịch/chạy MỌI test độc lập để file test API mới không bị bỏ quên.
test_count=0
while IFS= read -r test_cpp; do
    test_name="$(basename "${test_cpp}")"
    # shellcheck disable=SC2086
    if ! "${cxx}" ${flags} -I "${repow}" "$(win "${test_cpp}")" \
            -o "${tmpw}/test${test_count}.exe"; then
        printf 'error: biên dịch test thất bại: %s\n' "${test_name}" >&2
        exit 1
    fi
    if ! "${tmpw}/test${test_count}.exe" < /dev/null; then
        printf 'error: test chạy thất bại: %s\n' "${test_name}" >&2
        exit 1
    fi
    printf 'test passed: %s\n' "${test_name}"
    test_count=$((test_count + 1))
done < <(find "${lib}/tests" -maxdepth 1 -type f -name '*.cpp' | LC_ALL=C sort)
if [ "${test_count}" -eq 0 ]; then
    printf 'error: không tìm thấy cp_library/tests/*.cpp\n' >&2
    exit 1
fi
printf 'compiled + ran %d regression/property test file(s)\n' "${test_count}"

# 5. Dòng Tags của bài CSES đúng format/từ vựng; và dòng // CSES trong .hpp đã cập nhật.
"${python_cmd[@]}" "$(win "${tools_dir}/check_cses_tags.py")"
"${python_cmd[@]}" "$(win "${tools_dir}/link_cses.py")" --check

# 6. Tài liệu khớp hệ thống tệp: README/indexes phủ đủ snippet, không có liên kết chết,
#    mọi .hpp đủ header 5 nhãn + dòng // CSES + demo CP_DEMO + khối doc hàm.
"${python_cmd[@]}" "$(win "${tools_dir}/check_docs.py")"
"${python_cmd[@]}" "$(win "${tools_dir}/gen_docs.py")" --check

if [ "${skip_gnu_only}" = "1" ]; then
    printf 'cp_library verify passed; ordered-set.hpp chưa compile trên host này (GNU-only).\n'
else
    printf 'cp_library verify passed.\n'
fi
