import sys


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); pos += 1
    q = int(data[pos]); pos += 1

    # Chiều cao các tòa nhà, đánh chỉ số từ 1.
    h = [0] * (n + 2)
    for i in range(1, n + 1):
        h[i] = int(data[pos]); pos += 1

    INF = n + 1  # sentinel: luôn > b vì b <= n

    # nge[i] = tòa đầu tiên bên phải cao hơn NGHIÊM NGẶT tòa i, else INF.
    # Tính bằng monotonic stack O(n), duyệt từ phải sang trái.
    nge = [INF] * (n + 2)
    stack = []
    for i in range(n, 0, -1):
        hi = h[i]
        # Loại các tòa không cao hơn tòa hiện tại khỏi đỉnh stack.
        while stack and h[stack[-1]] <= hi:
            stack.pop()
        nge[i] = stack[-1] if stack else INF
        stack.append(i)
    nge[INF] = INF  # trạng thái hấp thụ cho sentinel

    # Bảng binary lifting: up[k][i] = vị trí sau khi nhảy 2^k bước nge từ i.
    LOG = 1
    while (1 << LOG) < n + 1:
        LOG += 1
    up = [nge]
    for k in range(1, LOG):
        prev = up[k - 1]
        cur = [0] * (n + 2)
        for i in range(1, n + 2):
            cur[i] = prev[prev[i]]
        up.append(cur)

    # Trả lời truy vấn: đếm số bước nhảy còn nằm trong [a, b].
    out = []
    ap = out.append
    for _ in range(q):
        a = int(data[pos]); pos += 1
        b = int(data[pos]); pos += 1
        node = a
        ans = 1  # tòa a luôn visible
        # Duyệt tầng từ cao xuống thấp, nhảy khi vẫn còn trong đoạn.
        for k in range(LOG - 1, -1, -1):
            nxt = up[k][node]
            if nxt <= b:
                node = nxt
                ans += 1 << k
        ap(ans)

    sys.stdout.buffer.write(("\n".join(map(str, out)) + "\n").encode())


main()
