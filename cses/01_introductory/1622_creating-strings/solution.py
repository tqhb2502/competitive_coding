import sys
from itertools import permutations


def main():
    data = sys.stdin.buffer.read().split()
    s = data[0].decode() if data else ""

    # Sinh mọi hoán vị từ các ký tự đã sắp xếp, loại trùng bằng set.
    distinct = set("".join(p) for p in permutations(sorted(s)))

    # Sắp xếp lại theo thứ tự từ điển để in ra.
    result = sorted(distinct)

    # In số lượng hoán vị phân biệt, rồi in từng xâu, mỗi xâu một dòng.
    out = [str(len(result))]
    out.extend(result)
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
