# Permutation Rounds - https://cses.fi/problemset/task/3398
# Số vòng (rounds) để mảng trở lại trạng thái sorted lần đầu tiên = LCM độ dài các cycle.
# In kết quả modulo 10^9+7.
import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    p = [0] * (n + 1)
    for i in range(1, n + 1):
        p[i] = int(data[idx]); idx += 1

    MOD = 10 ** 9 + 7

    # Tìm độ dài các cycle của permutation
    visited = bytearray(n + 1)
    cycle_lengths = []
    for i in range(1, n + 1):
        if not visited[i]:
            length = 0
            j = i
            while not visited[j]:
                visited[j] = 1
                j = p[j]
                length += 1
            cycle_lengths.append(length)

    # Smallest prime factor (SPF) sieve tới n
    max_len = n
    spf = list(range(max_len + 1))
    i = 2
    while i * i <= max_len:
        if spf[i] == i:
            for j in range(i * i, max_len + 1, i):
                if spf[j] == j:
                    spf[j] = i
        i += 1

    # LCM mod p: lấy số mũ lớn nhất cho mỗi prime trên tất cả cycle length
    prime_exp = {}
    for L in cycle_lengths:
        x = L
        while x > 1:
            pr = spf[x]
            cnt = 0
            while x % pr == 0:
                x //= pr
                cnt += 1
            if cnt > prime_exp.get(pr, 0):
                prime_exp[pr] = cnt

    ans = 1
    for pr, e in prime_exp.items():
        ans = (ans * pow(pr, e, MOD)) % MOD

    sys.stdout.write(str(ans % MOD) + "\n")


main()
