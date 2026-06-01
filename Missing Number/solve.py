import sys


def solve():
    input = sys.stdin.readline
    n = int(input())
    arr = list(map(int, input().split()))

    appeared = [False] * (2 * 10**5 + 1)
    for i in arr:
        appeared[i] = True

    for i in range(1, n + 1):
        if not appeared[i]:
            print(i)
            break

if __name__ == "__main__":
    solve()
