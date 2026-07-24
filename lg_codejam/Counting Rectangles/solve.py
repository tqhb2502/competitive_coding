import sys


def solve():
    data = list(map(int, sys.stdin.read().split()))
    N, M = data[0], data[1]
    values = data[2:]

    grid = []
    for i in range(N):
        grid.append(values[i * M : (i + 1) * M])

    # Đảo cột thành hàng (chuyển vị)
    if N > M:
        grid = [list(col) for col in zip(*grid)]
        N, M = M, N
    
    ans = 0
    for i in range(N - 1):
        for j in range(i + 1, N):
            appeared = [0] * 20
            for col in range(M):
                cur_sum = grid[i][col] + grid[j][col]
                needed = 20 - cur_sum
                ans += appeared[needed]
                appeared[cur_sum] += 1

    print(ans)


if __name__ == "__main__":
    solve()
