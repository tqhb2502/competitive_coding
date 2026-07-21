# Inverse Suffix Array - https://cses.fi/problemset/task/3225
#
# Cho trước suffix array SA (một hoán vị của các vị trí 1..n), hãy dựng lại
# một xâu bất kỳ trên bảng chữ cái a-z có đúng suffix array đó, hoặc in -1
# nếu không tồn tại xâu nào (cần quá 26 ký tự phân biệt).
#
# Ý tưởng (greedy inverse suffix array):
#   - rank[p] = thứ hạng của suffix bắt đầu tại p (rank[SA[i]] = i+1).
#     Quy ước rank[n+1] = 0 cho suffix rỗng (nhỏ nhất).
#   - Gán 'a' cho vị trí có suffix nhỏ nhất, rồi duyệt các suffix liên tiếp
#     theo thứ tự SA. Với cặp liên tiếp (p, q):
#       * Nếu rank[p+1] < rank[q+1] thì phần đuôi đã đúng thứ tự, giữ nguyên
#         ký tự (c[q] = c[p]).
#       * Ngược lại bắt buộc tăng ký tự (c[q] = c[p] + 1).
#   - Số ký tự phân biệt là ít nhất có thể. Nếu vượt quá 26 -> in -1.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    sa = data[1:1 + n]  # các vị trí 1-indexed (dạng bytes, chuyển int khi cần)
    sa = [int(x) for x in sa]

    # rank[p] cho p in 1..n; rank[n+1] = 0 (suffix rỗng)
    rank = [0] * (n + 2)
    for i in range(n):
        rank[sa[i]] = i + 1

    c = [0] * (n + 1)  # c[1..n]: chỉ số ký tự (0 = 'a')
    cur = 0
    c[sa[0]] = 0
    for i in range(1, n):
        p = sa[i - 1]
        q = sa[i]
        if rank[p + 1] < rank[q + 1]:
            c[q] = cur
        else:
            cur += 1
            c[q] = cur

    if cur > 25:
        sys.stdout.write("-1\n")
        return

    res = bytes(97 + c[i] for i in range(1, n + 1))
    sys.stdout.write(res.decode() + "\n")


main()
