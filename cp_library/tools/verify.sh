#!/usr/bin/env bash
# Kiểm chứng nhẹ cho kho snippet: mọi .hpp biên dịch độc lập, template chạy được,
# luồng bundle-1-file hoạt động, và dòng Tags CSES đúng format/từ vựng.
# YÊU CẦU: chạy bằng bash THẬT (git bash), không phải bash rút gọn của w64devkit.
#   export PATH="$PATH:$HOME/Downloads/w64devkit/bin"   # để tìm g++
#   bash cp_library/tools/verify.sh
set -euo pipefail

tools_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
lib="$(cd "${tools_dir}/.." && pwd)"
repo="$(cd "${lib}/.." && pwd)"
cxx="${CXX:-g++}"
flags="-std=c++17 -O2 -Wall -Wextra"

# g++ (bản Windows) cần path kiểu "d:/..." chứ không hiểu MSYS "/d/...".
win() { cygpath -m "$1" 2>/dev/null || printf '%s' "$1"; }

tmp="$(mktemp -d "${TMPDIR:-/tmp}/cp-verify.XXXXXX")"
trap 'rm -rf "${tmp}"' EXIT
tmpw="$(win "${tmp}")"
repow="$(win "${repo}")"

# 1. Mỗi snippet .hpp phải tự chứa: include vào driver rỗng rồi biên dịch.
count=0
while IFS= read -r hpp; do
    printf '#include "%s"\nint main() { return 0; }\n' "$(win "${hpp}")" > "${tmp}/driver.cpp"
    # shellcheck disable=SC2086
    "${cxx}" ${flags} "${tmpw}/driver.cpp" -o "${tmpw}/a.exe"
    # Demo #ifdef CP_DEMO: biên dịch .hpp làm file chính rồi chạy (mọi entry đều có demo).
    # shellcheck disable=SC2086
    "${cxx}" ${flags} -Wno-pragma-once-outside-header -DCP_DEMO -x c++ "$(win "${hpp}")" -o "${tmpw}/demo.exe"
    "${tmpw}/demo.exe" < /dev/null > /dev/null
    count=$((count + 1))
done < <(find "${lib}" -mindepth 2 -maxdepth 2 -name '*.hpp' | LC_ALL=C sort)
printf 'compiled + ran %d snippet header(s) và demo\n' "${count}"

# 2. Template thi đấu phải biên dịch và chạy an toàn không cần input.
for cpp in "${lib}"/templates/*.cpp; do
    # shellcheck disable=SC2086
    "${cxx}" ${flags} "$(win "${cpp}")" -o "${tmpw}/template.exe"
    "${tmpw}/template.exe" < /dev/null > /dev/null
done
printf 'templates compiled and ran\n'

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
python3 "$(win "${tools_dir}/bundle.py")" "${tmpw}/main.cpp" -I "${repow}" -o "${tmpw}/submit.cpp"
# shellcheck disable=SC2086
"${cxx}" ${flags} "${tmpw}/submit.cpp" -o "${tmpw}/submit.exe"
bundle_output="$("${tmpw}/submit.exe")"
if [ "${bundle_output}" != "1 3" ]; then
    printf 'error: bundle smoke output %q, expected "1 3"\n' "${bundle_output}" >&2
    exit 1
fi
printf 'bundle-to-one-file smoke passed\n'

# 4. Dòng Tags của bài CSES đúng format/từ vựng; và dòng // CSES trong .hpp đã cập nhật.
python3 "$(win "${tools_dir}/check_cses_tags.py")"
python3 "$(win "${tools_dir}/link_cses.py")" --check

printf 'cp_library verify passed.\n'
