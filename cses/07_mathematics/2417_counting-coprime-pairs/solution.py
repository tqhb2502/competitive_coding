import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    if n < 2:
        # Không đủ hai phần tử thì không có cặp nào.
        sys.stdout.write("0\n")
        return

    # Đọc các giá trị, đồng thời tìm maxv để giới hạn kích thước sàng.
    vals = data[1:1 + n]
    maxv = 0
    ivals = [0] * n
    for i in range(n):
        v = int(vals[i])
        ivals[i] = v
        if v > maxv:
            maxv = v

    # freq[v] = số lần giá trị v xuất hiện.
    freq = [0] * (maxv + 1)
    for v in ivals:
        freq[v] += 1

    N = maxv

    # Sàng Eratosthenes: đánh dấu số nguyên tố đến N.
    sieve = bytearray([1]) * (N + 1)
    sieve[0] = 0
    if N >= 1:
        sieve[1] = 0
    r = int(N ** 0.5)
    for i in range(2, r + 1):
        if sieve[i]:
            sieve[i * i::i] = bytearray(len(range(i * i, N + 1, i)))

    # Hàm Mobius mu[]:
    # mu[m] = (-1)^(số ước nguyên tố phân biệt) nếu m squarefree, ngược lại = 0.
    mu = [1] * (N + 1)
    mu[0] = 0
    for p in range(2, N + 1):
        if sieve[p]:
            # Đổi dấu tất cả bội của p (gộp một ước nguyên tố phân biệt).
            mu[p::p] = [-v for v in mu[p::p]]
            pp = p * p
            if pp <= N:
                # Số chia hết cho p^2 -> không squarefree -> mu = 0.
                mu[pp::pp] = [0] * len(range(pp, N + 1, pp))

    # ans = sum_d mu[d] * C(cnt[d], 2), với cnt[d] = số phần tử chia hết cho d.
    # Chỉ xét d có mu[d] != 0; cnt[d] tính bằng slicing sum (chạy ở tốc độ C).
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
