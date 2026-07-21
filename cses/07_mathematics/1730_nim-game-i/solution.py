# Nim Game I - CSES 1730
# https://cses.fi/problemset/task/1730
# Classic Nim: first player wins iff XOR of all heap sizes is nonzero
# (Bouton's theorem / Sprague-Grundy).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    t = int(data[idx]); idx += 1
    out = []
    for _ in range(t):
        n = int(data[idx]); idx += 1
        x = 0
        for j in range(idx, idx + n):
            x ^= int(data[j])
        idx += n
        out.append("first" if x != 0 else "second")
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
