# Required Substring - https://cses.fi/problemset/task/1112
# Dem so xau do dai n tren bang chu A-Z chua pattern nhu mot substring, mod 1e9+7.
# Y tuong: dung KMP automaton + DP dem so xau KHONG chua pattern,
# roi lay 26^n tru di. Trang thai = so ky tu cua pattern dang khop (0..m-1).
import sys

MOD = 10**9 + 7


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    pat = data[1] if len(data) > 1 else b""
    m = len(pat)

    # Ma hoa pattern ve 0..25 (A=0)
    P = [b - 65 for b in pat]

    # Prefix function (failure function) cua KMP
    pi = [0] * m
    for i in range(1, m):
        k = pi[i - 1]
        while k > 0 and P[i] != P[k]:
            k = pi[k - 1]
        if P[i] == P[k]:
            k += 1
        pi[i] = k

    # Xay KMP automaton: aut[j][c] = trang thai ke tiep tu trang thai j khi doc ky tu c.
    # j chay 0..m-1 (chua khop het). Neu aut[j][c] == m tuc la pattern da xuat hien.
    aut = [[0] * 26 for _ in range(m)]
    for j in range(m):
        pj = P[j]
        row = aut[j]
        if j == 0:
            for c in range(26):
                row[c] = 1 if c == pj else 0
        else:
            prev = aut[pi[j - 1]]
            for c in range(26):
                row[c] = j + 1 if c == pj else prev[c]

    # Nen chuyen tiep: voi moi trang thai j, gom cac ky tu dan toi cung mot trang thai < m.
    # (Bo cac ky tu dan toi m vi day la nhung xau BI loai - da chua pattern.)
    trans = []
    for j in range(m):
        cnt = {}
        row = aut[j]
        for c in range(26):
            nx = row[c]
            if nx < m:
                cnt[nx] = cnt.get(nx, 0) + 1
        trans.append(list(cnt.items()))

    # DP: dp[j] = so xau do dai hien tai, chua bao gio dat trang thai m, ket thuc o trang thai j.
    dp = [0] * m
    dp[0] = 1
    for _ in range(n):
        ndp = [0] * m
        for j in range(m):
            v = dp[j]
            if v:
                for nx, k in trans[j]:
                    ndp[nx] = (ndp[nx] + v * k) % MOD
        dp = ndp

    not_contain = sum(dp) % MOD
    ans = (pow(26, n, MOD) - not_contain) % MOD
    sys.stdout.write(str(ans) + "\n")


main()
