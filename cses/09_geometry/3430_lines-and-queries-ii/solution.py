import sys


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); pos += 1

    # Li Chao tree đặt trên trục vị trí, phủ [0, SZ) với SZ là lũy thừa 2 >= M+1.
    M = 100000
    SZ = 1
    while SZ < M + 1:
        SZ <<= 1                 # SZ = 131072 = 2^17
    H = SZ.bit_length() - 1      # chiều cao cây = 17

    LA = [None] * (2 * SZ)       # hệ số góc (slope) lưu tại mỗi nút (None = rỗng)
    LB = [None] * (2 * SZ)       # hệ số tự do (intercept) lưu tại mỗi nút

    out = []
    b1 = b'1'

    for _ in range(n):
        if data[pos] == b1:
            # Loại 1: thêm đường y = a*x + b active trên đoạn vị trí [l, r].
            a = int(data[pos + 1]); b = int(data[pos + 2])
            l = int(data[pos + 3]); r = int(data[pos + 4])
            pos += 5

            # Phân rã canonical [l, r] (nửa mở [l, r+1)) thành O(log M) nút chuẩn.
            lp = l + SZ
            rp = r + SZ + 1
            nodes = []
            while lp < rp:
                if lp & 1:
                    nodes.append(lp); lp += 1
                if rp & 1:
                    rp -= 1; nodes.append(rp)
                lp >>= 1; rp >>= 1

            for node0 in nodes:
                # Đoạn vị trí [lo, hi] mà nút chuẩn node0 phủ.
                L = node0.bit_length() - 1
                span = SZ >> L
                lo = (node0 - (1 << L)) * span
                hi = lo + span - 1
                # Li Chao insert đường (a, b) vào subtree gốc là node0.
                node = node0
                m = a; k = b
                while True:
                    cm = LA[node]
                    if cm is None:
                        # Nút trống: đặt luôn đường vào đây.
                        LA[node] = m; LB[node] = k
                        break
                    ck = LB[node]
                    mid = (lo + hi) >> 1
                    # So sánh tại điểm giữa mid: giữ đường tốt hơn ở nút.
                    if m * mid + k > cm * mid + ck:
                        LA[node] = m; LB[node] = k
                        wm = m; wk = k
                        m = cm; k = ck          # đường thua = đường cũ ở nút
                    else:
                        wm = cm; wk = ck        # đường thua = đường mới đưa vào
                    if lo == hi:
                        break
                    # Đẩy đường thua xuống nửa mà nó còn có thể vượt (xét đầu mút trái).
                    if m * lo + k > wm * lo + wk:
                        node = node * 2; hi = mid
                    else:
                        node = node * 2 + 1; lo = mid + 1
        else:
            # Loại 2: đi root -> leaf(x), lấy max các đường lưu trên đường đi.
            x = int(data[pos + 1]); pos += 2
            leaf = SZ + x
            best = None
            for s in range(H, -1, -1):
                node = leaf >> s
                a = LA[node]
                if a is not None:
                    v = a * x + LB[node]
                    if best is None or v > best:
                        best = v
            out.append(b"NO" if best is None else str(best).encode())

    sys.stdout.buffer.write(b"\n".join(out))
    if out:
        sys.stdout.buffer.write(b"\n")


main()
