import sys

MOD = 10**9 + 7


def solve():
    data = list(map(int, sys.stdin.read().split()))
    T = data[0]

    tests = []
    for i in range(T):
        G = data[2 * i + 1]
        D = data[2 * i + 2]
        tests.append((G, D))

    # Gom các test có D giống nhau
    groups = {}
    for idx, (G, D) in enumerate(tests):
        if D not in groups:
            groups[D] = []
        groups[D].append((idx, G))

    ans = [0] * T
    # Tính dựa trên từng giá trị D
    for D, items in groups.items():
        max_G = max(G for _, G in items)
        dp = [0] * (max_G + 1)
        dp[0] = 1

        sliding_window = dp[0]
        for i in range(1, max_G + 1):
            dp[i] += sliding_window

            sliding_window += dp[i]
            if sliding_window >= MOD:
                sliding_window -= MOD

            if i - D >= 0:
                sliding_window -= dp[i - D]
                if sliding_window < 0:
                    sliding_window += MOD

        for idx, G in items:
            ans[idx] = dp[G]
    
    for i in range(T):
        print(ans[i])


if __name__ == "__main__":
    solve()
