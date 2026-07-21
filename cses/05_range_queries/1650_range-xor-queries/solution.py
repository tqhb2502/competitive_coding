# Range Xor Queries - https://cses.fi/problemset/task/1650
# Static range XOR queries using a prefix-XOR array.
# XOR of [a, b] (1-indexed) = P[b] XOR P[a-1], where P is the prefix XOR.
# Build: O(n); each query: O(1).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    q = int(data[idx]); idx += 1

    # prefix[i] = x_1 XOR ... XOR x_i, prefix[0] = 0
    prefix = [0] * (n + 1)
    acc = 0
    for i in range(1, n + 1):
        acc ^= int(data[idx]); idx += 1
        prefix[i] = acc

    out = []
    out_append = out.append
    for _ in range(q):
        a = int(data[idx]); idx += 1
        b = int(data[idx]); idx += 1
        out_append(str(prefix[b] ^ prefix[a - 1]))

    sys.stdout.buffer.write(("\n".join(out) + "\n").encode())


if __name__ == "__main__":
    main()
