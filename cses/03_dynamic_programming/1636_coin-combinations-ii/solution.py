import sys

MOD = 1000000007


def main():
    # Đọc toàn bộ dữ liệu một lần cho nhanh.
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    x = int(data[1])

    # ways[s] = số cách (không kể thứ tự) tạo ra tổng s; base case ways[0] = 1.
    ways = [0] * (x + 1)
    ways[0] = 1

    # Gán tên hay dùng vào biến cục bộ để CPython tra cứu nhanh hơn.
    w = ways
    mod = MOD

    # Xét từng loại đồng xu ở vòng NGOÀI -> mỗi multiset chỉ đếm đúng một lần.
    for i in range(2, 2 + n):
        c = int(data[i])
        if c > x:
            continue  # Đồng xu lớn hơn tổng đích thì không đóng góp gì.
        # Cho s tăng dần để dùng lại loại xu c tùy ý (unbounded knapsack).
        for s in range(c, x + 1):
            v = w[s] + w[s - c]
            if v >= mod:
                v -= mod  # Trừ có điều kiện thay cho phép lấy dư (nhanh hơn).
            w[s] = v

    sys.stdout.write(str(w[x] % mod) + "\n")


main()
