import sys


def solve():
    data = list(map(int, sys.stdin.buffer.read().split()))
    N, M = data[0], data[1]

    D = data[2:2 + N]
    C = data[2 + N:2 + N + M]

    # a_i = D_i - 1
    max_a = max(D) - 1

    freq = [0] * (max_a + 1)
    for d in D:
        freq[d - 1] += 1

    # pref_cnt[x] = số lượng a <= x
    # pref_sum[x] = tổng các a <= x
    pref_cnt = [0] * (max_a + 1)
    pref_sum = [0] * (max_a + 1)

    cnt = 0
    total_sum = 0

    for value, f in enumerate(freq):
        cnt += f
        total_sum += value * f
        pref_cnt[value] = cnt
        pref_sum[value] = total_sum

    def get_le(x):
        """
        Trả về:
        - số lượng a <= x
        - tổng các a <= x
        """
        if x < 0:
            return 0, 0
        if x >= max_a:
            return N, total_sum
        return pref_cnt[x], pref_sum[x]

    # h = min(C_j - j) của các ngày trước.
    # Ban đầu chưa có ngày trước nào, tương đương +inf.
    # Nhưng vì mọi a <= max_a, ta có thể dùng h = max_a.
    h = max_a

    ans = []

    for day, cap in enumerate(C, start=1):
        q = cap - day

        if q >= h:
            ans.append("0")
        else:
            cnt_q, sum_q = get_le(q)
            cnt_h, sum_h = get_le(h)

            # Nhóm q < a <= h: đóng góp a - q
            mid_count = cnt_h - cnt_q
            mid_sum = sum_h - sum_q
            purge_mid = mid_sum - mid_count * q

            # Nhóm a > h: đóng góp h - q
            high_count = N - cnt_h
            purge_high = high_count * (h - q)

            ans.append(str(purge_mid + purge_high))

        # Cập nhật h cho các ngày sau
        if q < h:
            h = q

    sys.stdout.write("\n".join(ans))


if __name__ == "__main__":
    solve()
