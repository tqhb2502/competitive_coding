import sys


def solve():
    input = sys.stdin.buffer.readline

    N, K = map(int, input().split())

    events = []

    for _ in range(N):
        S, E, P, D = map(int, input().split())

        A = P + D * S

        # Item starts being active at S
        events.append((S, 1, A, D))

        # Item stops being active from E + 1
        events.append((E + 1, -1, -A, -D))

    events.sort()

    cnt = 0
    sumA = 0
    sumD = 0

    INF = 10**30
    ans = INF

    prev = events[0][0]
    i = 0
    m = len(events)

    while i < m:
        x = events[i][0]

        # Active set is unchanged on [prev, x - 1]
        if prev < x and cnt >= K:
            t = x - 1
            cost = sumA - sumD * t
            ans = min(ans, cost)

        # Process all events at time x
        while i < m and events[i][0] == x:
            _, dc, dA, dD = events[i]
            cnt += dc
            sumA += dA
            sumD += dD
            i += 1

        prev = x

    print(-1 if ans == INF else ans)


if __name__ == "__main__":
    solve()
