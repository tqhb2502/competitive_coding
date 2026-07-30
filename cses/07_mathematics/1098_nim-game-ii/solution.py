import sys


def main():
    # Đọc toàn bộ input một lần cho nhanh với t và n lớn.
    data = sys.stdin.buffer.read().split()
    idx = 0
    t = int(data[idx]); idx += 1
    out = []
    for _ in range(t):
        n = int(data[idx]); idx += 1
        # Grundy của mỗi đống theo subtraction game {1,2,3} là x % 4,
        # XOR toàn bộ để lấy giá trị Grundy của cả ván.
        g = 0
        end = idx + n
        while idx < end:
            g ^= int(data[idx]) & 3  # x % 4 tương đương x & 3
            idx += 1
        # XOR khác 0 -> first thắng, bằng 0 -> second thắng.
        out.append("first" if g != 0 else "second")
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
