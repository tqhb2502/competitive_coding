import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    q = int(data[idx]); idx += 1

    INF = float('inf')
    N = n
    # Hai iterative segment tree với 2*N nút; lá nằm ở vị trí [N, 2N).
    # Tách trị tuyệt đối: L lưu p_a - a (dùng prefix-min), R lưu p_a + a (dùng suffix-min).
    L = [INF] * (2 * N)  # lưu p_a - a
    R = [INF] * (2 * N)  # lưu p_a + a

    # Nạp giá vào các lá theo chỉ số 1-indexed.
    for i in range(N):
        p = int(data[idx]); idx += 1
        a = i + 1  # 1-indexed
        L[N + i] = p - a
        R[N + i] = p + a

    # Dựng min ngược lên gốc cho cả hai cây.
    for i in range(N - 1, 0, -1):
        li = 2 * i
        L[i] = L[li] if L[li] < L[li + 1] else L[li + 1]
        R[i] = R[li] if R[li] < R[li + 1] else R[li + 1]

    out = []
    out_append = out.append

    for _ in range(q):
        t = data[idx]; idx += 1
        if t == b'2':
            # Truy vấn: chi phí nhỏ nhất khi đứng ở tòa k.
            k = int(data[idx]); idx += 1

            # prefix-min L trên [1, k] -> nửa khoảng 0-indexed [0, k), rồi cộng k.
            res = INF
            l = N
            r = k + N
            while l < r:
                if l & 1:
                    v = L[l]
                    if v < res:
                        res = v
                    l += 1
                if r & 1:
                    r -= 1
                    v = L[r]
                    if v < res:
                        res = v
                l >>= 1
                r >>= 1
            ans1 = res + k

            # suffix-min R trên [k, n] -> nửa khoảng 0-indexed [k-1, n), rồi trừ k.
            res = INF
            l = (k - 1) + N
            r = n + N
            while l < r:
                if l & 1:
                    v = R[l]
                    if v < res:
                        res = v
                    l += 1
                if r & 1:
                    r -= 1
                    v = R[r]
                    if v < res:
                        res = v
                l >>= 1
                r >>= 1
            ans2 = res - k

            # Đáp án là min của hai hướng trái/phải.
            out_append(ans1 if ans1 < ans2 else ans2)
        else:
            # Cập nhật: gán p_k = x, cập nhật cả hai cây rồi đẩy min lên gốc.
            k = int(data[idx]); idx += 1
            x = int(data[idx]); idx += 1
            i = k - 1

            # Cập nhật cây L với giá trị mới x - k.
            p = N + i
            L[p] = x - k
            p >>= 1
            while p:
                lp = 2 * p
                a = L[lp]; b = L[lp + 1]
                L[p] = a if a < b else b
                p >>= 1

            # Cập nhật cây R với giá trị mới x + k.
            p = N + i
            R[p] = x + k
            p >>= 1
            while p:
                lp = 2 * p
                a = R[lp]; b = R[lp + 1]
                R[p] = a if a < b else b
                p >>= 1

    sys.stdout.buffer.write(('\n'.join(map(str, out)) + ('\n' if out else '')).encode())


main()
