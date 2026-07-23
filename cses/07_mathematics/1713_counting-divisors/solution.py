import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    nums = [int(v) for v in data[1:1 + n]]
    if not nums:
        return

    maxv = max(nums)

    # smallest prime factor sieve: spf[i] là ước nguyên tố nhỏ nhất của i.
    # Khởi tạo spf[i] = i, rồi với mỗi nguyên tố i đánh dấu các bội chưa chạm.
    spf = list(range(maxv + 1))
    i = 2
    while i * i <= maxv:
        if spf[i] == i:  # i là số nguyên tố
            step = i
            for j in range(i * i, maxv + 1, step):
                if spf[j] == j:
                    spf[j] = i
        i += 1

    # Với mỗi x: liên tục chia cho spf[x] để lấy số mũ e của từng nguyên tố,
    # số ước là tích các (e + 1).
    out = []
    append = out.append
    for x in nums:
        d = 1
        while x > 1:
            p = spf[x]
            e = 0
            while x % p == 0:
                x //= p
                e += 1
            d *= e + 1
        append(d)

    sys.stdout.write('\n'.join(map(str, out)) + '\n')


main()
