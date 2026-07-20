# Creating Strings - CSES 1622
# https://cses.fi/problemset/task/1622

import sys
from itertools import permutations


def main():
    data = sys.stdin.buffer.read().split()
    s = data[0].decode() if data else ""

    # Sinh mọi hoán vị từ các ký tự đã sort, loại trùng bằng set.
    distinct = set("".join(p) for p in permutations(sorted(s)))
    result = sorted(distinct)

    out = [str(len(result))]
    out.extend(result)
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
