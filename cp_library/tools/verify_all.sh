#!/usr/bin/env bash

set -euo pipefail

tools_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
library_root="$(cd "${tools_dir}/.." && pwd)"
compiler="${CXX:-c++}"
temporary_dir="$(mktemp -d "${TMPDIR:-/tmp}/cp-library-verify.XXXXXX")"
trap 'rm -rf "${temporary_dir}"' EXIT

compile_count=0
compiled_program=""

compile_cpp() {
    local source_file="$1"
    local output_file="${temporary_dir}/program-${compile_count}"
    compile_count=$((compile_count + 1))

    "${compiler}" \
        -std=c++17 \
        -O2 \
        -Wall \
        -Wextra \
        -Wpedantic \
        -Wshadow \
        -Werror \
        -I"${library_root}" \
        "${source_file}" \
        -o "${output_file}"
    compiled_program="${output_file}"
}

python3 "${tools_dir}/tests/test_tools.py"
python3 "${tools_dir}/build_index.py" --check
python3 "${tools_dir}/check_cses_tags.py"

# Các template độc lập đều phải biên dịch và chạy an toàn không cần input.
while IFS= read -r source_file; do
    compile_cpp "${source_file}"
    "${compiled_program}" >/dev/null
done < <(
    find "${library_root}/templates" -maxdepth 1 -type f -name '*.cpp' -print |
        LC_ALL=C sort
)

# Entry mẫu phải biên dịch/chạy như một entry thật; example có output cố định.
template_entry="${library_root}/_templates/entry"
compile_cpp "${template_entry}/example.cpp"
example_output="$("${compiled_program}")"
if [[ "${example_output}" != "10" ]]; then
    printf 'error: template example output is %q, expected 10\n' \
        "${example_output}" >&2
    exit 1
fi
compile_cpp "${template_entry}/test.cpp"
"${compiled_program}"

# Catalog là nguồn chuẩn: compile header độc lập, compile example và chạy mọi
# test đã khai báo trong metadata, kể cả khi test không mang tên test.cpp.
while IFS=$'\t' read -r file_kind relative_path; do
    case "${file_kind}" in
        source)
            header_driver="${temporary_dir}/header-${compile_count}.cpp"
            printf '#include "%s"\nint main() { return 0; }\n' \
                "${relative_path}" >"${header_driver}"
            compile_cpp "${header_driver}"
            ;;
        example)
            compile_cpp "${library_root}/${relative_path}"
            "${compiled_program}" >/dev/null
            ;;
        test)
            compile_cpp "${library_root}/${relative_path}"
            "${compiled_program}"
            ;;
        *)
            printf 'error: unknown catalog file kind: %s\n' \
                "${file_kind}" >&2
            exit 1
            ;;
    esac
done < <(
    python3 - "${library_root}/indexes/catalog.json" <<'PYTHON'
import json
import sys

with open(sys.argv[1], "r", encoding="utf-8") as input_file:
    catalog = json.load(input_file)

for entry in catalog["entries"]:
    print("source\t{}".format(entry["source"]))
    print("example\t{}".format(entry["example"]))
    for test_path in entry["tests"]:
        print("test\t{}".format(test_path))
PYTHON
)

printf 'CP library verification passed.\n'
