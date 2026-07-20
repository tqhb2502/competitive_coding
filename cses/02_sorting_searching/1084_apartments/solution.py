# Apartments - CSES 1084
# https://cses.fi/problemset/task/1084
# Greedy + two-pointer: sort applicants and apartments, then match greedily.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1
    k = int(data[idx]); idx += 1

    a = data[idx:idx + n]
    idx += n
    b = data[idx:idx + m]
    idx += m

    a = sorted(map(int, a))
    b = sorted(map(int, b))

    i = 0  # con trỏ ứng viên (applicant pointer)
    j = 0  # con trỏ căn hộ (apartment pointer)
    count = 0
    while i < n and j < m:
        if b[j] < a[i] - k:
            # Căn hộ quá nhỏ, bỏ qua
            j += 1
        elif b[j] > a[i] + k:
            # Ứng viên không thể nhận căn hộ nào nữa
            i += 1
        else:
            # Ghép được một cặp
            count += 1
            i += 1
            j += 1

    sys.stdout.write(str(count) + "\n")


main()
