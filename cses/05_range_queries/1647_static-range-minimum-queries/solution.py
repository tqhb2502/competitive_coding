import sys


def main():
    # Đọc toàn bộ input một lần để tối ưu tốc độ I/O.
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    q = int(data[1])

    base = 2
    x = [int(v) for v in data[base:base + n]]

    # Sparse Table: sp[k][i] = min của đoạn [i, i + 2^k - 1].
    # Mỗi hàng ghép hai đoạn 2^(k-1) kề nhau qua zip(prev, prev[half:]).
    sp = [x]
    k = 1
    while (1 << k) <= n:
        prev = sp[k - 1]
        half = 1 << (k - 1)
        cur = [a if a < b else b for a, b in zip(prev, prev[half:])]
        sp.append(cur)
        k += 1

    # Bảng log2 nguyên: log[i] = floor(log2(i)).
    log = [0] * (n + 1)
    for i in range(2, n + 1):
        log[i] = log[i >> 1] + 1

    ptr = base + n
    out = []
    ap = out.append
    for _ in range(q):
        a = int(data[ptr]) - 1  # đổi truy vấn về 0-indexed
        b = int(data[ptr + 1]) - 1
        ptr += 2
        # Phủ [a, b] bằng hai đoạn độ dài 2^kk (có thể chồng lấn), min idempotent.
        kk = log[b - a + 1]
        row = sp[kk]
        v1 = row[a]
        v2 = row[b - (1 << kk) + 1]
        ap(v1 if v1 < v2 else v2)

    # Ghi toàn bộ kết quả một lần, kết thúc bằng LF.
    sys.stdout.buffer.write(('\n'.join(map(str, out)) + '\n').encode())


main()
