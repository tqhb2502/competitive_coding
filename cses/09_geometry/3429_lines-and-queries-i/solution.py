import sys


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); pos += 1

    N = 100000                 # miền của x là [0, N]
    size = 4 * (N + 1)
    NEG = -(1 << 62)           # số rất âm, luôn thua mọi đường thật (~1e14)

    A = [0] * size             # hệ số góc (slope) lưu tại mỗi node
    B = [NEG] * size           # hệ số tự do (intercept); node rỗng = NEG

    out = []

    for _ in range(n):
        t = data[pos]; pos += 1
        if t == b'1':
            # "1 a b": chèn đường y = nm*x + nb (Li Chao insert, đi một nhánh xuống lá)
            nm = int(data[pos]); nb = int(data[pos + 1]); pos += 2
            node = 1
            l = 0
            r = N
            while True:
                m = (l + r) >> 1
                cm = A[node]; cb = B[node]
                # Giữ lại đường lớn hơn tại trung điểm m ở node, đẩy đường thua xuống.
                if nm * m + nb > cm * m + cb:
                    A[node] = nm; B[node] = nb   # đường mới thắng tại m
                    ws = nm                       # winner slope
                    nm, nb = cm, cb               # đường cũ trở thành đường thua bị đẩy
                else:
                    ws = cm                       # đường cũ (vẫn ở node) thắng
                    # (nm, nb) đã là đường thua cần đẩy xuống
                if l == r:
                    break
                # Đường thua chỉ vượt đường thắng ở đúng một phía của m; phía đó
                # quyết định bởi slope: slope nhỏ hơn -> trội ở nửa trái (x < m).
                if nm < ws:
                    node += node          # đi con trái
                    r = m
                else:
                    node += node + 1      # đi con phải
                    l = m + 1
        else:
            # "2 x": truy vấn max f(x) dọc đường đi từ gốc xuống lá x
            x = int(data[pos]); pos += 1
            node = 1
            l = 0
            r = N
            best = NEG
            while True:
                v = A[node] * x + B[node]
                if v > best:
                    best = v
                if l == r:
                    break
                m = (l + r) >> 1
                if x <= m:
                    node += node
                    r = m
                else:
                    node += node + 1
                    l = m + 1
            out.append(best)

    sys.stdout.write('\n'.join(map(str, out)))
    if out:
        sys.stdout.write('\n')


main()
