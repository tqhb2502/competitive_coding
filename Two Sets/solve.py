import sys


def solve():
    input = sys.stdin.readline
    n = int(input())

    if (n * (n + 1) // 2) % 2 == 1:
        print("NO")
        return;

    half_sum_value = n * (n + 1) // 4
    can_divide = False
    first_set_count = 0

    for v in range(n, 0, -1):
        if half_sum_value >= v:
            half_sum_value -= v
            first_set_count += 1
        if half_sum_value == 0:
            can_divide = True
            break

    if can_divide:
        print("YES")
        print(first_set_count)

        half_sum_value = n * (n + 1) // 4
        printed = [False] * (n + 1)

        for v in range(n, 0, -1):
            if half_sum_value >= v:
                half_sum_value -= v
                print(v, end=" ")
                printed[v] = True
        
        print()
        print(n - first_set_count)
        for v in range(1, n + 1):
            if not printed[v]:
                print(v, end=" ")
    else:
        print("NO")


if __name__ == "__main__":
    solve()
