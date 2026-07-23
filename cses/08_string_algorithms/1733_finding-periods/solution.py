import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    s = data[0]  # bytes; lập chỉ mục trả về int (nhanh)
    n = len(s)

    # KMP prefix function; dùng biến cục bộ cho nhanh khi n tới 1e6
    # pi[i] = độ dài border dài nhất của tiền tố s[0..i]
    pi = [0] * n
    k = 0
    for i in range(1, n):
        c = s[i]
        # Lùi theo border chain cho tới khi khớp được ký tự hoặc về 0
        while k > 0 and s[k] != c:
            k = pi[k - 1]
        if s[k] == c:
            k += 1
        pi[i] = k

    # Đi dọc border chain -> các period theo thứ tự tăng dần (border giảm dần)
    res = []
    b = pi[n - 1]
    while b > 0:
        res.append(n - b)
        b = pi[b - 1]
    res.append(n)  # cả xâu (border rỗng) luôn là một period

    sys.stdout.write(" ".join(map(str, res)))
    sys.stdout.write("\n")


main()
