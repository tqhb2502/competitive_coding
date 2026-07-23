import sys


def main():
    # Đọc trực tiếp theo bytes để tăng tốc trên input tới 10^6 ký tự.
    data = sys.stdin.buffer.read().split()
    if len(data) < 2:
        sys.stdout.write("0\n")
        return
    t = data[0]  # chuỗi văn bản text (dạng bytes)
    p = data[1]  # chuỗi mẫu pattern (dạng bytes)

    n = len(t)
    m = len(p)
    if m > n:
        sys.stdout.write("0\n")
        return

    # Bước 1: tính failure function (prefix function) của pattern.
    pi = [0] * m
    k = 0
    for i in range(1, m):
        c = p[i]
        while k and p[k] != c:
            k = pi[k - 1]
        if p[k] == c:
            k += 1
        pi[i] = k

    # Bước 2: quét text, đếm số lần khớp đầy đủ (kể cả chồng lấn).
    ans = 0
    k = 0
    for i in range(n):
        c = t[i]
        while k and p[k] != c:
            k = pi[k - 1]
        if p[k] == c:
            k += 1
            if k == m:
                ans += 1
                k = pi[k - 1]

    sys.stdout.write(str(ans) + "\n")


main()
