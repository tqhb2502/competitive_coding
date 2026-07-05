import sys


def solve():
    input = sys.stdin.readline

    N = int(input())
    A = list(map(int, input().split()))

    # prefix[i] = tổng A[0] đến A[i-1]
    prefix = [0] * (N + 1)
    for i in range(N):
        prefix[i + 1] = prefix[i] + A[i]

    # left_greater[i] = vị trí gần nhất bên trái có A[j] > A[i]
    left_greater = [-1] * N
    stack = []

    for i in range(N):
        while stack and A[stack[-1]] <= A[i]:
            stack.pop()

        if stack:
            left_greater[i] = stack[-1]

        stack.append(i)

    # right_greater[i] = vị trí gần nhất bên phải có A[j] > A[i]
    right_greater = [N] * N
    stack = []

    for i in range(N - 1, -1, -1):
        while stack and A[stack[-1]] <= A[i]:
            stack.pop()

        if stack:
            right_greater[i] = stack[-1]

        stack.append(i)

    # Segment tree cho min và max trên prefix
    size = 1
    while size < N + 1:
        size *= 2

    INF = 10**30

    seg_min = [INF] * (2 * size)
    seg_max = [-INF] * (2 * size)

    for i in range(N + 1):
        seg_min[size + i] = prefix[i]
        seg_max[size + i] = prefix[i]

    for i in range(size - 1, 0, -1):
        seg_min[i] = min(seg_min[i * 2], seg_min[i * 2 + 1])
        seg_max[i] = max(seg_max[i * 2], seg_max[i * 2 + 1])

    def query_min(l, r):
        l += size
        r += size
        result = INF

        while l <= r:
            if l % 2 == 1:
                result = min(result, seg_min[l])
                l += 1
            if r % 2 == 0:
                result = min(result, seg_min[r])
                r -= 1

            l //= 2
            r //= 2

        return result

    def query_max(l, r):
        l += size
        r += size
        result = -INF

        while l <= r:
            if l % 2 == 1:
                result = max(result, seg_max[l])
                l += 1
            if r % 2 == 0:
                result = max(result, seg_max[r])
                r -= 1

            l //= 2
            r //= 2

        return result

    answer = 0

    for i in range(N):
        # Đoạn hợp lệ quanh i
        left_bound = left_greater[i] + 1
        right_bound = right_greater[i]

        # Tổng lớn nhất bên trái i
        left_best = prefix[i] - query_min(left_bound, i)

        # Tổng lớn nhất bên phải i
        right_best = query_max(i + 1, right_bound) - prefix[i + 1]

        answer = max(answer, left_best + right_best)

    print(answer)


if __name__ == "__main__":
    solve()
