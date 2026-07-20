# Two Sets - CSES 1092
# https://cses.fi/problemset/task/1092

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])

    # Total sum of 1..n is n*(n+1)/2. A valid split needs it to be even,
    # which happens exactly when n % 4 == 0 or n % 4 == 3.
    if n % 4 != 0 and n % 4 != 3:
        sys.stdout.write("NO\n")
        return

    target = n * (n + 1) // 4  # required sum for each set

    set1 = []
    set2 = []
    remaining = target
    # Greedy from the largest number downward: take i whenever it still fits.
    for i in range(n, 0, -1):
        if i <= remaining:
            set1.append(i)
            remaining -= i
        else:
            set2.append(i)

    out = []
    out.append("YES")
    out.append(str(len(set1)))
    out.append(" ".join(map(str, set1)))
    out.append(str(len(set2)))
    out.append(" ".join(map(str, set2)))
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
