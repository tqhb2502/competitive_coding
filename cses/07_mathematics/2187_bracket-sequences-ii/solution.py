# Bracket Sequences II - https://cses.fi/problemset/task/2187
# Đếm số dãy ngoặc hợp lệ độ dài n có prefix cho trước, modulo 1e9+7.
# Lattice path + reflection principle:
#   sau prefix ta ở độ cao b, còn m = n - k bước, đi về 0 không xuống dưới 0.
#   kết quả = C(m, (m-b)/2) - C(m, (m+b+2)/2) (mod p).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    s = data[1] if len(data) > 1 else b""
    k = len(s)

    MOD = 10 ** 9 + 7

    # n lẻ -> không thể cân bằng
    if n % 2 == 1:
        sys.stdout.write("0\n")
        return

    # Tính balance của prefix; nếu âm tại bất kỳ vị trí nào -> 0
    OPEN = ord("(")
    bal = 0
    ok = True
    for ch in s:
        if ch == OPEN:
            bal += 1
        else:
            bal -= 1
        if bal < 0:
            ok = False
            break

    if not ok:
        sys.stdout.write("0\n")
        return

    b = bal
    m = n - k

    # Nếu không thể đi về 0: b > m hoặc chênh lệch lẻ -> C tự trả 0, nhưng
    # ta vẫn tính bình thường qua hàm comb.

    # Tiền xử lý factorial tới n
    fact = [1] * (n + 1)
    for i in range(1, n + 1):
        fact[i] = fact[i - 1] * i % MOD
    inv_fact = [1] * (n + 1)
    inv_fact[n] = pow(fact[n], MOD - 2, MOD)
    for i in range(n, 0, -1):
        inv_fact[i - 1] = inv_fact[i] * i % MOD

    def comb(N, R):
        if R < 0 or R > N:
            return 0
        return fact[N] * inv_fact[R] % MOD * inv_fact[N - R] % MOD

    # (m - b) phải chẵn và >= 0 để có đường đi
    if (m - b) % 2 != 0 or m - b < 0:
        sys.stdout.write("0\n")
        return

    ans = (comb(m, (m - b) // 2) - comb(m, (m + b + 2) // 2)) % MOD
    sys.stdout.write(str(ans % MOD) + "\n")


main()
