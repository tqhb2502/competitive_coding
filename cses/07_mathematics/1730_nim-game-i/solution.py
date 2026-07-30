import sys


def main():
    # Đọc toàn bộ input một lần cho nhanh
    data = sys.stdin.buffer.read().split()
    idx = 0
    t = int(data[idx]); idx += 1
    out = []
    for _ in range(t):
        n = int(data[idx]); idx += 1
        # Tính Nim sum: XOR dồn kích thước tất cả các đống
        x = 0
        for j in range(idx, idx + n):
            x ^= int(data[j])
        idx += n
        # Bouton's theorem: XOR != 0 thì người đi trước thắng, ngược lại người đi sau thắng
        out.append("first" if x != 0 else "second")
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
