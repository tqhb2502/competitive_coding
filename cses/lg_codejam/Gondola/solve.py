import sys


def solve():
    input = sys.stdin.readline

    N, K = map(int, input().split())
    A = list(map(int, input().split()))

    if N < 2 * K:
        print(-1)
        return

    # Đếm số lượng component của một đoạn active có độ dài length
    def comp_count(length):
        if length == 0:
            return 0
        if length <= K:
            return length
        if length < 2 * K:
            return 2 * K - length + 1
        return 1

    active = [False] * N

    # Nếu một đoạn active bắt đầu tại l thì right_end[l] = r
    right_end = [-1] * N

    # Nếu một đoạn active kết thúc tại r thì left_start[r] = l
    left_start = [-1] * N

    # Sắp xếp mảng
    order = sorted(range(N), key=lambda i: A[i])

    components = N
    answer = 0

    for idx in order:
        h = A[idx]

        left_len = 0
        right_len = 0

        new_l = idx
        new_r = idx

        # Có đoạn active bên trái
        if idx > 0 and active[idx - 1]:
            l = left_start[idx - 1]
            r = idx - 1
            left_len = r - l + 1
            new_l = l

        # Có đoạn active bên phải
        if idx + 1 < N and active[idx + 1]:
            l = idx + 1
            r = right_end[idx + 1]
            right_len = r - l + 1
            new_r = r

        before = 1 + comp_count(left_len) + comp_count(right_len)
        after = comp_count(left_len + 1 + right_len)

        delta = before - after

        # Các cạnh nối các comp cũ phải đi ra đỉnh hiện tại,
        # do không thể đi qua các đỉnh trước vì khoảng cách chỉ số < K.
        # Do vậy nhân thêm với h hiện tại.
        components -= delta
        answer += delta * h

        active[idx] = True

        right_end[new_l] = new_r
        left_start[new_r] = new_l

    if components == 1:
        print(answer)
    else:
        print(-1)


if __name__ == "__main__":
    solve()
