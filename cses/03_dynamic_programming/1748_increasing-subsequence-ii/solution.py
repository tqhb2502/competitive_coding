import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    a = data[1:1 + n]

    MOD = 1000000007

    # Nén tọa độ: gán rank 1..m cho các giá trị phân biệt, tính sẵn rank cho từng phần tử
    # để tránh tra cứu dict trong vòng lặp chính.
    ints = list(map(int, a))
    sorted_vals = sorted(set(ints))
    rank = {v: i + 1 for i, v in enumerate(sorted_vals)}
    m = len(sorted_vals)
    ranks = [rank[v] for v in ints]

    # Fenwick tree (BIT) lưu tổng dp theo rank giá trị.
    # dp luôn được rút gọn < MOD nên mỗi ô tree bị cộng tối đa n lần, giá trị < n*MOD ~ 2e14,
    # vẫn nằm trong phạm vi số nguyên máy (< 2^63). Nhờ đó bỏ được phép mod trong vòng update,
    # giảm hằng số thời gian mà không sợ tràn (mọi phép chỉ là cộng nên lấy mod ở cuối là đủ).
    tree = [0] * (m + 1)

    total = 0
    # Duyệt trái sang phải: khi xử lý phần tử i thì BIT chỉ chứa các j < i.
    for r in ranks:
        # Truy vấn prefix sum trên [1, r-1] -> tổng dp của các phần tử trước có giá trị < v
        # (dừng ở r-1 để bảo đảm tăng NGẶT).
        i = r - 1
        s = 0
        while i > 0:
            s += tree[i]
            i -= i & (-i)
        # dp[i] = 1 + tổng dp[j] thỏa điều kiện; số 1 ứng với dãy chỉ gồm riêng phần tử này.
        dp = (s + 1) % MOD
        total += dp
        # Cập nhật: cộng dp vào vị trí rank r (không rút gọn tree, giá trị vẫn bị chặn an toàn).
        i = r
        while i <= m:
            tree[i] += dp
            i += i & (-i)

    sys.stdout.write(str(total % MOD))


main()
