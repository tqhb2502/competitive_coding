#!/usr/bin/env bash
# Stress test: so lời giải chính với lời giải ngây thơ trên test ngẫu nhiên, dừng ngay khi lệch.
# Dùng:  bash cp_library/templates/stress.sh [sol.cpp] [brute.cpp] [gen.cpp] [số vòng]
# Mặc định: main.cpp brute.cpp gen.cpp 500
# YÊU CẦU: bash thật (Git Bash) + g++ trên PATH
# (Windows: export PATH="$HOME/Downloads/w64devkit/bin:$PATH").
set -uo pipefail

SOL=${1:-main.cpp}
BRUTE=${2:-brute.cpp}
GEN=${3:-gen.cpp}
ROUNDS=${4:-500}
CXX=${CXX:-g++}
FLAGS="-std=c++17 -O2 -Wall -Wextra"

if ! [[ "${ROUNDS}" =~ ^[1-9][0-9]*$ ]]; then
    printf 'số vòng phải là số nguyên dương, nhận: %s\n' "${ROUNDS}" >&2
    exit 1
fi

for src in "${SOL}" "${BRUTE}" "${GEN}"; do
    [ -f "${src}" ] || { printf 'không thấy tệp: %s\n' "${src}" >&2; exit 1; }
done

tmp="$(mktemp -d "${TMPDIR:-/tmp}/cp-stress.XXXXXX")"
trap 'rm -rf "${tmp}"' EXIT
# g++ bản Windows cần đường dẫn kiểu "d:/..." thay vì MSYS "/d/...".
win() { cygpath -m "$1" 2>/dev/null || printf '%s' "$1"; }
tmpw="$(win "${tmp}")"

# shellcheck disable=SC2086
"${CXX}" ${FLAGS} "$(win "${SOL}")"   -o "${tmpw}/sol.exe"   || exit 1
# shellcheck disable=SC2086
"${CXX}" ${FLAGS} "$(win "${BRUTE}")" -o "${tmpw}/brute.exe" || exit 1
# shellcheck disable=SC2086
"${CXX}" ${FLAGS} "$(win "${GEN}")"   -o "${tmpw}/gen.exe"   || exit 1

for ((i = 1; i <= ROUNDS; i++)); do
    if ! "${tmp}/gen.exe" "${i}" > "${tmp}/in.txt"; then
        printf 'generator CRASH ở vòng %d\n' "${i}" >&2
        exit 1
    fi
    if ! "${tmp}/sol.exe" < "${tmp}/in.txt" > "${tmp}/out_sol.txt"; then
        printf 'lời giải chính CRASH ở vòng %d — input:\n' "${i}"; cat "${tmp}/in.txt"; exit 1
    fi
    if ! "${tmp}/brute.exe" < "${tmp}/in.txt" > "${tmp}/out_brute.txt"; then
        printf 'brute CRASH ở vòng %d — input:\n' "${i}" >&2
        cat "${tmp}/in.txt" >&2
        exit 1
    fi
    # Judge chuẩn so dãy token (bỏ khác biệt khoảng trắng), KHÔNG được dùng
    # `diff -w`: tùy chọn đó còn xóa separator và coi "1 2" giống "12".
    LC_ALL=C awk '{ for (i = 1; i <= NF; ++i) print $i }' \
        "${tmp}/out_sol.txt" > "${tmp}/tokens_sol.txt"
    LC_ALL=C awk '{ for (i = 1; i <= NF; ++i) print $i }' \
        "${tmp}/out_brute.txt" > "${tmp}/tokens_brute.txt"
    if ! diff -q "${tmp}/tokens_sol.txt" "${tmp}/tokens_brute.txt" > /dev/null; then
        printf 'LỆCH ở vòng %d\n--- input ---\n' "${i}"; cat "${tmp}/in.txt"
        printf -- '--- sol ---\n'; cat "${tmp}/out_sol.txt"
        printf -- '--- brute ---\n'; cat "${tmp}/out_brute.txt"
        exit 1
    fi
    if (( i % 100 == 0 )); then printf '  khớp %d/%d\n' "${i}" "${ROUNDS}"; fi
done
printf 'Khớp toàn bộ %d vòng.\n' "${ROUNDS}"
