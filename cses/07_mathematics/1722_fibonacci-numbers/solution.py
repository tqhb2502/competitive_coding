import sys

MOD = 10 ** 9 + 7


def fib_pair(n):
    # Fast doubling: trả về cặp (F(n), F(n+1)) mod MOD.
    if n == 0:
        return (0, 1)
    # a = F(k), b = F(k+1) với k = n // 2
    a, b = fib_pair(n >> 1)
    # Hai đẳng thức fast doubling
    c = (a * ((2 * b - a) % MOD)) % MOD          # F(2k)
    d = (a * a + b * b) % MOD                     # F(2k+1)
    # Nếu bit hiện tại là 1 thì dịch thêm một bước Fibonacci
    if n & 1:
        return (d, (c + d) % MOD)                 # (F(2k+1), F(2k+2))
    else:
        return (c, d)                             # (F(2k), F(2k+1))


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    sys.stdout.write(str(fib_pair(n)[0] % MOD) + "\n")


if __name__ == "__main__":
    main()
