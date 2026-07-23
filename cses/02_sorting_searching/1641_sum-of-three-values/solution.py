import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    x = int(data[1])

    # Ghép giá trị với chỉ số gốc (đánh số từ 1), rồi sort theo giá trị tăng dần.
    pairs = sorted((int(data[2 + i]), i + 1) for i in range(n))
    # Tách thành hai mảng số nguyên để truy cập trong vòng lặp nhanh hơn nhiều
    # so với index vào tuple như arr[l][0].
    vals = [p[0] for p in pairs]
    idxs = [p[1] for p in pairs]

    write = sys.stdout.write
    # Tổng cặp lớn nhất có thể có trong bất kỳ hậu tố [i+1, n-1] nào.
    hi = vals[n - 1] + vals[n - 2] if n >= 2 else 0

    # Cố định phần tử đầu tiên vals[i], quy về bài toán 2SUM trên đoạn [i+1, n-1].
    for i in range(n - 2):
        vi = vals[i]
        need = x - vi
        # Cắt tỉa: không cặp nào trong hậu tố đạt tới need -> bỏ qua i này.
        if need > hi:
            continue
        # Cắt tỉa: cặp nhỏ nhất trong hậu tố đã vượt need. Vì need giảm dần còn
        # tổng cặp nhỏ nhất tăng dần theo i nên từ đây về sau đều vô vọng.
        if need < vals[i + 1] + vals[i + 2]:
            break
        # Two-pointer trên đoạn đã sort để tìm cặp có tổng bằng need.
        l = i + 1
        r = n - 1
        while l < r:
            s = vals[l] + vals[r]
            if s == need:
                # Tìm được bộ ba, in ra ba chỉ số gốc.
                write(f"{idxs[i]} {idxs[l]} {idxs[r]}\n")
                return
            elif s < need:
                l += 1  # Cần tổng lớn hơn.
            else:
                r -= 1  # Cần tổng nhỏ hơn.

    # Duyệt hết mà không có bộ ba nào thỏa mãn.
    write("IMPOSSIBLE\n")


main()
