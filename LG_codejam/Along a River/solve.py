import sys


def solve():
    data = list(map(int, sys.stdin.read().split()))
    N = data[0]
    K = data[1]
    S = data[2 : N + 2]
    E = data[N + 2 :]

    # Số cửa hàng bắt đầu mở hoặc đóng lại thời điểm t
    change_at = [0] * (K + 2)
    for i in range(N):
        change_at[S[i]] += 1
        change_at[E[i] + 1] -= 1

    # Số cặp cửa hàng liên tiếp (i và i + 1) CÙNG* bắt đầu mở hoặc đóng tại thời điểm t
    # CÙNG*: có thể 1 cửa hàng mở trc, 1 cửa hàng mở sau, sau khi cả 2 cùng mở như vậy, mới đc tính là 1
    prev_sum_adj = [0] * (K + 2)
    for i in range(N - 1):
        adj_start = max(S[i], S[i + 1])
        adj_end = min(E[i], E[i + 1])
        if adj_start <= adj_end:
            prev_sum_adj[adj_start] += 1
            prev_sum_adj[adj_end + 1] -= 1

    # In kết quả
    res = 0
    for i in range(1, K + 1):
        res += change_at[i] - prev_sum_adj[i]
        print(res, end=" ")


if __name__ == "__main__":
    solve()
