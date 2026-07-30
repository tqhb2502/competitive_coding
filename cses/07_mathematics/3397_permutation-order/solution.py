import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    t = int(data[idx]); idx += 1

    # Tiền xử lý giai thừa: fact[i] = i! cho i = 0..20.
    fact = [1] * 21
    for i in range(1, 21):
        fact[i] = fact[i - 1] * i

    out = []
    for _ in range(t):
        qtype = data[idx]; idx += 1
        n = int(data[idx]); idx += 1

        if qtype == b'1':
            # Loại 1: giải mã biểu diễn giai thừa của k-1 thành hoán vị.
            k = int(data[idx]); idx += 1
            kk = k - 1  # đổi sang đánh số từ 0
            available = list(range(1, n + 1))  # luôn giữ thứ tự tăng dần
            res = []
            for i in range(n, 0, -1):
                f = fact[i - 1]      # số hoán vị của i-1 phần tử còn lại
                j = kk // f          # chỉ số phần tử được chọn trong available
                kk %= f
                res.append(available.pop(j))
            out.append(' '.join(map(str, res)))
        else:
            # Loại 2: mã hóa hoán vị thành thứ hạng.
            perm = [int(data[idx + i]) for i in range(n)]
            idx += n
            available = list(range(1, n + 1))  # luôn giữ thứ tự tăng dần
            rank = 0
            for i in range(n):
                x = perm[i]
                pos = available.index(x)  # số phần tử còn lại nhỏ hơn x
                # Mỗi phần tử nhỏ hơn đứng trước ứng với fact[...] hoán vị đứng trước.
                rank += pos * fact[n - 1 - i]
                available.pop(pos)
            out.append(str(rank + 1))  # đổi lại về đánh số từ 1

    sys.stdout.write('\n'.join(out) + '\n')


main()
