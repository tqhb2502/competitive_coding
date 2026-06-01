import sys


def solve():
    input = sys.stdin.readline
    N = int(input().strip())
    print(N, end=" ")
    
    while (N != 1):
        if N % 2 == 0:
            N = N // 2
        else:
            N = N * 3 + 1
        print(N, end=" ")


if __name__ == "__main__":
    solve()
