# Pizzeria Queries — CSES 2206
# https://cses.fi/problemset/task/2206
#
# Chi phí từ tòa a đến tòa k = p_a + |a - k|.
#   a <= k: (p_a - a) + k  -> prefix-min của L[a] = p_a - a trên [1, k]
#   a >= k: (p_a + a) - k  -> suffix-min của R[a] = p_a + a trên [k, n]
# Dùng hai iterative segment tree (range-min, point update). Fenwick min không
# dùng được vì cập nhật gán giá trị bất kỳ (có thể tăng lẫn giảm).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    q = int(data[idx]); idx += 1

    INF = float('inf')
    N = n
    # Iterative segment tree với 2*N nút; lá ở vị trí [N, 2N).
    L = [INF] * (2 * N)  # lưu p_a - a
    R = [INF] * (2 * N)  # lưu p_a + a

    for i in range(N):
        p = int(data[idx]); idx += 1
        a = i + 1  # 1-indexed
        L[N + i] = p - a
        R[N + i] = p + a

    for i in range(N - 1, 0, -1):
        li = 2 * i
        L[i] = L[li] if L[li] < L[li + 1] else L[li + 1]
        R[i] = R[li] if R[li] < R[li + 1] else R[li + 1]

    out = []
    out_append = out.append

    for _ in range(q):
        t = data[idx]; idx += 1
        if t == b'2':
            k = int(data[idx]); idx += 1

            # prefix-min L trên [1, k] -> nửa khoảng 0-indexed [0, k)
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

            # suffix-min R trên [k, n] -> nửa khoảng 0-indexed [k-1, n)
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

            out_append(ans1 if ans1 < ans2 else ans2)
        else:
            k = int(data[idx]); idx += 1
            x = int(data[idx]); idx += 1
            i = k - 1

            p = N + i
            L[p] = x - k
            p >>= 1
            while p:
                lp = 2 * p
                a = L[lp]; b = L[lp + 1]
                L[p] = a if a < b else b
                p >>= 1

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
