# Palindrome Reorder - CSES 1755
# https://cses.fi/problemset/task/1755

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    s = data[0]  # bytes, characters A-Z

    # Count frequency of each of the 26 uppercase letters.
    counts = [0] * 26
    for b in s:
        counts[b - 65] += 1  # ord('A') == 65

    odd_index = -1
    odd_count = 0
    for i in range(26):
        if counts[i] & 1:
            odd_count += 1
            odd_index = i

    if odd_count > 1:
        sys.stdout.write("NO SOLUTION\n")
        return

    # Build the left half from half of each character's count (A -> Z order).
    half_parts = []
    for i in range(26):
        if counts[i] >= 2:
            half_parts.append(chr(65 + i) * (counts[i] // 2))
    left = "".join(half_parts)

    middle = chr(65 + odd_index) if odd_index != -1 else ""

    result = left + middle + left[::-1]
    sys.stdout.write(result)
    sys.stdout.write("\n")


main()
