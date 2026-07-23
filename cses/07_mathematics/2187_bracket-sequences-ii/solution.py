import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    s = data[1] if len(data) > 1 else b""
    k = len(s)

    MOD = 10 ** 9 + 7

    # Bước 1: n lẻ thì không thể cân bằng, đáp án là 0.
    if n % 2 == 1:
        sys.stdout.write("0\n")
        return

    # Bước 2: tính balance của prefix (mỗi '(' là +1, mỗi ')' là -1); nếu âm
    # tại bất kỳ vị trí nào thì prefix không hợp lệ, đáp án là 0.
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

    # Bước 3: sau prefix ta ở độ cao b, còn m = n - k bước đi về 0 không xuống
    # dưới 0.
    b = bal
    m = n - k

    # Tiền xử lý factorial và inverse factorial theo modulo tới n (nghịch đảo
    # dùng Fermat vì MOD là số nguyên tố).
    fact = [1] * (n + 1)
    for i in range(1, n + 1):
        fact[i] = fact[i - 1] * i % MOD
    inv_fact = [1] * (n + 1)
    inv_fact[n] = pow(fact[n], MOD - 2, MOD)
    for i in range(n, 0, -1):
        inv_fact[i - 1] = inv_fact[i] * i % MOD

    # Tổ hợp C(N, R), trả 0 nếu R nằm ngoài khoảng [0, N].
    def comb(N, R):
        if R < 0 or R > N:
            return 0
        return fact[N] * inv_fact[R] % MOD * inv_fact[N - R] % MOD

    # (m - b) phải chẵn và >= 0 thì mới có đường đi hợp lệ.
    if (m - b) % 2 != 0 or m - b < 0:
        sys.stdout.write("0\n")
        return

    # Reflection principle: số đường tự do trừ số đường chạm mức -1.
    ans = (comb(m, (m - b) // 2) - comb(m, (m + b + 2) // 2)) % MOD
    sys.stdout.write(str(ans % MOD) + "\n")


main()
