import sys


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); pos += 1

    MOD = 10 ** 9 + 7
    PHI = MOD - 1  # dùng cho Fermat: x^e = x^(e mod PHI) khi gcd(x, MOD) = 1

    xs = [0] * n
    ks = [0] * n
    for i in range(n):
        xs[i] = int(data[pos])
        ks[i] = int(data[pos + 1])
        pos += 2

    # (1) Số lượng ước: tích (k_i + 1) mod MOD.
    divcount = 1
    for k in ks:
        divcount = divcount * ((k + 1) % MOD) % MOD

    # (2) Tổng ước: tích (x^{k+1} - 1) * nghịch_đảo(x - 1) mod MOD (chuỗi hình học).
    # Vì 2 <= x <= 1e6 < MOD nên x - 1 thuộc [1, 1e6-1], không bao giờ chia hết cho
    # MOD -> nghịch đảo luôn tồn tại (không cần xét x == 1 mod MOD).
    # Nghịch đảo hàng loạt (thủ thuật Montgomery): thay n phép pow(., MOD-2) bằng
    # đúng một phép pow cộng O(n) phép nhân, giảm mạnh nguy cơ TLE với pure Python.
    denom = [(x - 1) % MOD for x in xs]      # đều thuộc [1, 1e6-1], khác 0
    prefix = [1] * n                          # prefix[i] = tích denom[0..i-1]
    acc = 1
    for i in range(n):
        prefix[i] = acc
        acc = acc * denom[i] % MOD
    inv_acc = pow(acc, MOD - 2, MOD)          # nghịch đảo của tích toàn bộ denom
    inv_denom = [0] * n
    for i in range(n - 1, -1, -1):            # quét ngược lấy từng nghịch đảo
        inv_denom[i] = inv_acc * prefix[i] % MOD
        inv_acc = inv_acc * denom[i] % MOD

    sumdiv = 1
    for i in range(n):
        num = (pow(xs[i], ks[i] + 1, MOD) - 1) % MOD
        sumdiv = sumdiv * (num * inv_denom[i] % MOD) % MOD

    # (3) Tích ước: tích x_i^{k_i * D / 2}, với D = số ước (nguyên).
    # Xử lý phép chia 2 sao cho luôn làm việc với số mũ nguyên:
    #   - Nếu D chẵn (tồn tại k_i lẻ -> k_i+1 chẵn): chia đôi đúng một thừa số chẵn
    #     (k_j+1) của D, phần còn lại giữ nguyên; khi đó E_i = k_i * (D/2).
    #   - Nếu D lẻ (mọi k_i đều chẵn): E_i = (k_i/2) * D.
    # Sau đó rút gọn số mũ theo mod PHI (Fermat). Không chia 2 trực tiếp mod PHI vì
    # PHI = 1e9+6 chẵn nên 2 không khả nghịch mod PHI.
    D_even = any((k & 1) == 1 for k in ks)  # D chẵn <=> có ít nhất một k_i lẻ

    prod = 1
    if D_even:
        # halfmod = (D/2) mod PHI, chia đôi một thừa số (k_j+1) chẵn duy nhất
        halfmod = 1
        halved = False
        for k in ks:
            f = k + 1
            if (not halved) and (f % 2 == 0):
                halfmod = halfmod * ((f // 2) % PHI) % PHI
                halved = True
            else:
                halfmod = halfmod * (f % PHI) % PHI
        for i in range(n):
            e = (ks[i] % PHI) * halfmod % PHI  # e = k_i * (D/2) mod PHI
            prod = prod * pow(xs[i], e, MOD) % MOD
    else:
        Dmod = 1
        for k in ks:
            Dmod = Dmod * ((k + 1) % PHI) % PHI
        for i in range(n):
            e = ((ks[i] // 2) % PHI) * Dmod % PHI  # e = (k_i/2) * D mod PHI
            prod = prod * pow(xs[i], e, MOD) % MOD

    sys.stdout.write("%d %d %d\n" % (divcount, sumdiv, prod))


main()
