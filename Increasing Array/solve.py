import sys


def solve():
    input = sys.stdin.readline
    n = int(input())
    arr = list(map(int, input().split()))

    result = 0
    cur = arr[0]
    for val in arr[1:]:
        if val >= cur:
            cur = val
        else:
            result += cur - val

    print(result)


if __name__ == "__main__":
    solve()
