import sys


def main():
    # Doc toan bo input mot lan cho nhanh.
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    m = int(data[1])
    MOD = 1000000007

    # dp[v] = so cach dien tien to sao cho phan tu cuoi mang gia tri v.
    # Do dai m+2, chi so 0 va m+1 la o dem (padding) luon = 0 de tranh kiem tra bien.
    dp = [0] * (m + 2)

    # Co so (i = 0): x[0] == 0 thi moi v deu duoc phep, nguoc lai chi dung x[0].
    x0 = int(data[2])
    if x0 == 0:
        for v in range(1, m + 1):
            dp[v] = 1
    else:
        dp[x0] = 1

    # cnt: dem so buoc da hoan mod (defer modulo) lien tiep.
    cnt = 0
    for i in range(1, n):
        xi = int(data[2 + i])
        if xi == 0:
            # Vi tri chua biet: cong ba lat cat (slice) cua list cung luc bang zip.
            # core[k] = dp[k] + dp[k+1] + dp[k+2] => dp_new[v] = dp[v-1] + dp[v] + dp[v+1].
            core = [a + b + c for a, b, c in zip(dp, dp[1:], dp[2:])]
            cnt += 1
            if cnt == 16:
                # Sau 16 buoc, gia tri < 3^16 * 1e9 ~ 4.3e16; Python int chinh xac tuyet doi.
                core = [v % MOD for v in core]
                cnt = 0
            # Bao lai hai o dem quanh core de giu vung padding.
            dp = [0]
            dp += core
            dp.append(0)
        else:
            # Vi tri da biet: chi tinh dung o xi, lay modulo ngay va reset bo dem.
            val = (dp[xi - 1] + dp[xi] + dp[xi + 1]) % MOD
            dp = [0] * (m + 2)
            dp[xi] = val
            cnt = 0

    # Dap an: tong dp[v] (lay modulo).
    sys.stdout.write(str(sum(dp) % MOD) + "\n")


main()
