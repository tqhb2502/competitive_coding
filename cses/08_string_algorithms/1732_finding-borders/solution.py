import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        sys.stdout.write("\n")
        return
    s = data[0]                    # dạng bytes; s[i] là số nguyên trong Python 3
    n = len(s)

    # Prefix function (hàm thất bại của KMP), O(n) amortized:
    # pi[i] là độ dài prefix thực sự dài nhất của s[0..i] cũng là suffix của nó.
    pi = [0] * n
    k = 0
    for i in range(1, n):
        c = s[i]
        # Lùi theo chuỗi pi cho tới khi ký tự khớp hoặc về 0.
        while k > 0 and s[k] != c:
            k = pi[k - 1]
        if s[k] == c:
            k += 1
        pi[i] = k

    # Lần theo chuỗi pi từ cuối xâu để thu mọi độ dài border (giảm dần).
    res = []
    b = pi[n - 1]
    while b > 0:
        res.append(b)
        b = pi[b - 1]
    # Đảo ngược để in theo thứ tự tăng dần.
    res.reverse()

    sys.stdout.write(" ".join(map(str, res)) + "\n")


main()
