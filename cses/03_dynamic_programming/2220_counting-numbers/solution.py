# Counting Numbers - CSES 2220
# https://cses.fi/problemset/task/2220
# Digit DP: đếm các số trong [a, b] không có hai chữ số liền kề bằng nhau.
# Đáp án = count(b) - count(a - 1), với count(x) đếm trong [0, x].

import sys
from functools import lru_cache


def count(x):
    # Số các số nguyên n với 0 <= n <= x không có hai chữ số liền kề bằng nhau.
    if x < 0:
        return 0
    s = str(x)
    n = len(s)

    @lru_cache(maxsize=None)
    def dp(pos, prev, started, tight):
        if pos == n:
            # Kết thúc: đếm số vừa tạo (kể cả số 0 khi toàn bộ là leading zero).
            return 1
        limit = int(s[pos]) if tight else 9
        total = 0
        for d in range(0, limit + 1):
            ntight = tight and (d == limit)
            if not started:
                if d == 0:
                    # Vẫn đang ở vùng leading zero: prev = 10 nghĩa là "chưa có".
                    total += dp(pos + 1, 10, False, ntight)
                else:
                    total += dp(pos + 1, d, True, ntight)
            else:
                if d == prev:
                    continue
                total += dp(pos + 1, d, True, ntight)
        return total

    result = dp(0, 10, False, True)
    dp.cache_clear()
    return result


def main():
    data = sys.stdin.buffer.read().split()
    a = int(data[0])
    b = int(data[1])
    sys.stdout.write(str(count(b) - count(a - 1)) + "\n")


if __name__ == "__main__":
    main()
