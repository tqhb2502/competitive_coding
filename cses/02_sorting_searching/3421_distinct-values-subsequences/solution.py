import sys
from collections import Counter


def main():
    # Đọc toàn bộ dữ liệu bằng bộ đệm để tăng tốc.
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    xs = data[1:1 + n]
    MOD = 10 ** 9 + 7

    # Đếm số lần xuất hiện của từng giá trị (token phân biệt tương ứng giá trị phân biệt).
    cnt = Counter(xs)

    # Nhân dồn tích (1 + cnt[v]) trên mọi giá trị phân biệt, theo modulo.
    ans = 1
    for c in cnt.values():
        ans = ans * (1 + c) % MOD

    # Trừ 1 để loại bỏ dãy con rỗng.
    ans = (ans - 1) % MOD
    sys.stdout.write(str(ans) + "\n")


main()
