import sys


def sum_pairwise_abs(values):
    # Bài toán một chiều: tổng |a_i - a_j| trên mọi cặp bằng sort + prefix sum.
    # Sắp xếp tăng dần rồi cộng dồn đóng góp a[k]*k - (tổng a[0..k-1]) qua mọi k.
    values.sort()
    total = 0
    prefix = 0  # Tổng tiền tố a[0..k-1].
    for k, v in enumerate(values):
        total += v * k - prefix
        prefix += v
    return total


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        sys.stdout.write("0\n")
        return
    n = int(data[0])
    xs = [0] * n
    ys = [0] * n
    idx = 1
    for i in range(n):
        xs[i] = int(data[idx])
        ys[i] = int(data[idx + 1])
        idx += 2
    # Tách độc lập theo chiều x và chiều y rồi cộng lại; big int của Python cho
    # kết quả chính xác tuyệt đối, không tràn số.
    answer = sum_pairwise_abs(xs) + sum_pairwise_abs(ys)
    sys.stdout.write(str(answer) + "\n")


if __name__ == "__main__":
    main()
