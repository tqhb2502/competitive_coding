import sys


def solve(y: int , x: int):
    line = max(y, x)
    max_in_line = line**2
    is_even_line = line % 2 == 0

    result = 0
    # Dòng chẵn
    if is_even_line:
        result = max_in_line - x + 1
        if y < x:
            result -= (x - y)
    else: # Dòng lẻ
        result = max_in_line - y + 1
        if x < y:
            result -= (y - x)
    
    print(result)


if __name__ == "__main__":
    input = sys.stdin.readline
    N = int(input())
    while (N):
        y, x = map(int, input().split())
        solve(y, x)
        N -= 1
