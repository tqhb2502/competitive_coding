# Prime Multiples - https://cses.fi/problemset/task/2185
# Đếm số nguyên trong [1, n] chia hết cho ít nhất một trong k số nguyên tố.
# Dùng nguyên lý bao hàm - loại trừ (inclusion-exclusion), có cắt tỉa tích > n.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    k = int(data[1])
    primes = [int(x) for x in data[2:2 + k]]

    answer = 0
    # dp lưu (tích_tập_con, số_phần_tử); bắt đầu bằng tập rỗng (tích = 1, cnt = 0)
    dp = [(1, 0)]
    for p in primes:
        additions = []
        for prod, cnt in dp:
            np = prod * p
            if np <= n:
                new_cnt = cnt + 1
                additions.append((np, new_cnt))
                if new_cnt & 1:
                    answer += n // np
                else:
                    answer -= n // np
        dp.extend(additions)

    sys.stdout.write(str(answer) + "\n")


main()
