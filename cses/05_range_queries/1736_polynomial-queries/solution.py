# Polynomial Queries - https://cses.fi/problemset/task/1736
#
# Update loại 1 (a b): cộng 1 vào vị trí a, 2 vào a+1, 3 vào a+2, ... (một cấp số cộng).
# Update loại 2 (a b): trả về tổng các giá trị trong [a, b].
#
# Ý tưởng: giá trị cộng thêm tại vị trí i (với một update [l, r]) là inc(i) = i - l + 1.
# Lấy sai phân bậc hai f của hàm inc thì f chỉ khác 0 tại 3 điểm:
#     f[l]   += 1
#     f[r+1] += -(r - l + 2)
#     f[r+2] += (r - l + 1)
# Khi đó d[i] = sum_{j<=i} f[j]*(i - j + 1), và prefix sum của delta:
#     PS(x) = sum_{i<=x} d[i]
#           = (1/2) * [ (x^2+3x+2)*A0(x) - (2x+3)*A1(x) + A2(x) ]
# với A0(x)=sum_{j<=x} f[j], A1(x)=sum_{j<=x} j*f[j], A2(x)=sum_{j<=x} j^2*f[j].
# Dùng 3 Fenwick/BIT cho A0, A1, A2 (point update, prefix query). Giá trị ban đầu
# được cộng riêng qua prefix sum tĩnh (pre[]). Độ phức tạp O((n+q) log n).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    q = int(data[1])
    pos = 2

    # Prefix sum của mảng ban đầu (tĩnh, không bao giờ thay đổi).
    pre = [0] * (n + 1)
    s = 0
    for i in range(1, n + 1):
        s += int(data[pos]); pos += 1
        pre[i] = s

    # Ba Fenwick tree lưu f[j], j*f[j], j^2*f[j]. Kích thước n là đủ vì các
    # query prefix chỉ đến n; các point update tại r+1, r+2 vượt quá n bị bỏ qua.
    a0 = [0] * (n + 1)
    a1 = [0] * (n + 1)
    a2 = [0] * (n + 1)

    out = []
    ap = out.append

    for _ in range(q):
        t = data[pos]; a = int(data[pos + 1]); b = int(data[pos + 2]); pos += 3
        if t == b'1':
            l = a; r = b
            # Điểm l: (v0,v1,v2) = (1, l, l*l)
            i = l; v1 = l; v2 = l * l
            while i <= n:
                a0[i] += 1; a1[i] += v1; a2[i] += v2
                i += i & (-i)
            # Điểm r+1: c1 = -(r-l+2)
            p = r + 1
            if p <= n:
                c1 = -(r - l + 2); w1 = p * c1; w2 = p * p * c1
                i = p
                while i <= n:
                    a0[i] += c1; a1[i] += w1; a2[i] += w2
                    i += i & (-i)
            # Điểm r+2: c2 = (r-l+1)
            p = r + 2
            if p <= n:
                c2 = r - l + 1; w1 = p * c2; w2 = p * p * c2
                i = p
                while i <= n:
                    a0[i] += c2; a1[i] += w1; a2[i] += w2
                    i += i & (-i)
        else:
            l = a; r = b
            # PS(r)
            x = r; s0 = 0; s1 = 0; s2 = 0; i = x
            while i > 0:
                s0 += a0[i]; s1 += a1[i]; s2 += a2[i]
                i -= i & (-i)
            psr = ((x * x + 3 * x + 2) * s0 - (2 * x + 3) * s1 + s2) // 2
            # PS(l-1)
            x = l - 1
            if x > 0:
                s0 = 0; s1 = 0; s2 = 0; i = x
                while i > 0:
                    s0 += a0[i]; s1 += a1[i]; s2 += a2[i]
                    i -= i & (-i)
                psl = ((x * x + 3 * x + 2) * s0 - (2 * x + 3) * s1 + s2) // 2
            else:
                psl = 0
            ap(psr - psl + pre[r] - pre[l - 1])

    sys.stdout.buffer.write(('\n'.join(map(str, out)) + '\n').encode())


main()
