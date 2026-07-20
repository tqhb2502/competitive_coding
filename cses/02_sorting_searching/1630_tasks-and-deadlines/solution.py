# Tasks and Deadlines - CSES 1630
# https://cses.fi/problemset/task/1630
# Greedy: reward = sum(d) - sum(f). sum(d) is constant, so minimize sum(f)
# by doing tasks with shortest duration a first (Shortest Processing Time).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1

    tasks = []
    for _ in range(n):
        a = int(data[idx]); d = int(data[idx + 1]); idx += 2
        tasks.append((a, d))

    # Sort by duration ascending (SPT rule).
    tasks.sort(key=lambda x: x[0])

    t = 0          # current finishing time
    ans = 0        # total reward
    for a, d in tasks:
        t += a
        ans += d - t

    sys.stdout.write(str(ans) + "\n")


if __name__ == "__main__":
    main()
