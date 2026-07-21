# Counting Coprime Pairs - https://cses.fi/problemset/task/2417
# Đếm số cặp (i, j), i < j, sao cho gcd(x_i, x_j) = 1.
#
# Ý tưởng: dùng hàm Mobius.
#   Gọi cnt[d] = số phần tử chia hết cho d.
#   Số cặp mà CẢ HAI phần tử đều chia hết cho d là C(cnt[d], 2).
#   Theo Mobius inversion, số cặp coprime = sum_d mu(d) * C(cnt[d], 2).
#
# cnt[d] = sum(freq[d], freq[2d], freq[3d], ...)  (tính bằng slicing sum, tốc độ C).
# mu(d) tính bằng sàng nguyên tố.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    if n < 2:
        sys.stdout.write("0\n")
        return

    vals = data[1:1 + n]
    maxv = 0
    ivals = [0] * n
    for i in range(n):
        v = int(vals[i])
        ivals[i] = v
        if v > maxv:
            maxv = v

    freq = [0] * (maxv + 1)
    for v in ivals:
        freq[v] += 1

    N = maxv

    # --- Sàng Eratosthenes (đánh dấu số nguyên tố) ---
    sieve = bytearray([1]) * (N + 1)
    sieve[0] = 0
    if N >= 1:
        sieve[1] = 0
    r = int(N ** 0.5)
    for i in range(2, r + 1):
        if sieve[i]:
            sieve[i * i::i] = bytearray(len(range(i * i, N + 1, i)))

    # --- Hàm Mobius mu[] ---
    # mu[m] = (-1)^(số ước nguyên tố phân biệt) nếu m squarefree, ngược lại = 0.
    mu = [1] * (N + 1)
    mu[0] = 0
    for p in range(2, N + 1):
        if sieve[p]:
            # đổi dấu tất cả bội của p (mỗi ước nguyên tố gộp một lần)
            mu[p::p] = [-v for v in mu[p::p]]
            pp = p * p
            if pp <= N:
                # số không squarefree (chia hết cho p^2) -> mu = 0
                mu[pp::pp] = [0] * len(range(pp, N + 1, pp))

    # --- Tổng Mobius: ans = sum_d mu[d] * C(cnt[d], 2) ---
    ans = 0
    fr = freq
    md = mu
    for d in range(1, N + 1):
        m = md[d]
        if m:
            c = sum(fr[d::d])          # cnt[d]
            if c > 1:
                ans += m * (c * (c - 1) // 2)

    sys.stdout.write(str(ans) + "\n")


main()
