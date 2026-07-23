import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    p = [0] * (n + 1)
    for i in range(1, n + 1):
        p[i] = int(data[idx]); idx += 1

    MOD = 10 ** 9 + 7

    # Tách hoán vị thành các cycle: duyệt i -> p[i] và đánh dấu visited.
    # Đáp án là bậc của hoán vị = LCM độ dài các cycle.
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

    # SPF sieve (smallest prime factor) tới n để factorize nhanh mỗi độ dài cycle
    max_len = n
    spf = list(range(max_len + 1))
    i = 2
    while i * i <= max_len:
        if spf[i] == i:
            for j in range(i * i, max_len + 1, i):
                if spf[j] == j:
                    spf[j] = i
        i += 1

    # LCM qua phân tích thừa số: với mỗi prime lấy số mũ lớn nhất
    # xuất hiện trên tất cả các độ dài cycle
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

    # LCM mod p = tích của prime^(số mũ lớn nhất) theo modulo MOD
    ans = 1
    for pr, e in prime_exp.items():
        ans = (ans * pow(pr, e, MOD)) % MOD

    sys.stdout.write(str(ans % MOD) + "\n")


main()
