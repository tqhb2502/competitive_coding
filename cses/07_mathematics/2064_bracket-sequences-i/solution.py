import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])

    MOD = 1_000_000_007

    # n lẻ thì không thể có dãy ngoặc hợp lệ nào.
    if n % 2 == 1:
        sys.stdout.write("0\n")
        return

    m = n // 2  # số cặp ngoặc

    # Tiền xử lý giai thừa: fact[k] = k! mod MOD.
    fact = [1] * (n + 1)
    for i in range(1, n + 1):
        fact[i] = fact[i - 1] * i % MOD

    # Nghịch đảo giai thừa: dùng định lý Fermat (Fermat's little theorem) cho
    # phần tử cuối rồi truy hồi ngược inv_fact[i-1] = inv_fact[i] * i % MOD.
    inv_fact = [1] * (n + 1)
    inv_fact[n] = pow(fact[n], MOD - 2, MOD)
    for i in range(n, 0, -1):
        inv_fact[i - 1] = inv_fact[i] * i % MOD

    # Đáp án là số Catalan thứ m: Catalan(m) = (2m)! / (m! * (m+1)!), với 2m = n.
    ans = fact[n] * inv_fact[m] % MOD * inv_fact[m + 1] % MOD

    sys.stdout.write(str(ans) + "\n")


main()
