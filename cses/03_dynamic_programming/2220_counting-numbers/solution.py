import sys
from functools import lru_cache


def count(x):
    # Đếm các số nguyên n với 0 <= n <= x không có hai chữ số liền kề bằng nhau.
    if x < 0:
        return 0
    s = str(x)
    n = len(s)

    # Digit DP: duyệt các chữ số từ trái sang phải theo trạng thái
    # (pos, prev, started, tight). prev = 10 nghĩa là chưa đặt chữ số nào.
    @lru_cache(maxsize=None)
    def dp(pos, prev, started, tight):
        if pos == n:
            # Cơ sở: đặt hết chữ số, số vừa tạo hợp lệ (kể cả số 0 khi toàn bộ
            # là leading zero) nên đếm là 1.
            return 1
        # Khi tight thì chữ số bị chặn trên bởi chữ số tương ứng của x.
        limit = int(s[pos]) if tight else 9
        total = 0
        for d in range(0, limit + 1):
            ntight = tight and (d == limit)
            if not started:
                if d == 0:
                    # Vẫn là leading zero: chưa bắt đầu số, giữ prev = 10.
                    total += dp(pos + 1, 10, False, ntight)
                else:
                    total += dp(pos + 1, d, True, ntight)
            else:
                # Đã bắt đầu số: cấm chữ số trùng chữ số liền trước.
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
    # Đáp án cho đoạn [a, b] tính bằng hiệu hai giá trị tiền tố.
    sys.stdout.write(str(count(b) - count(a - 1)) + "\n")


if __name__ == "__main__":
    main()
