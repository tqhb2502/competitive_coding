import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    sa = [int(x) for x in data[1:1 + n]]  # các vị trí 1-indexed

    # rank[p] = thứ hạng của suffix bắt đầu tại p (rank[SA[i]] = i + 1);
    # rank[n+1] = 0 dành cho suffix rỗng (nhỏ nhất).
    rank = [0] * (n + 2)
    for i in range(n):
        rank[sa[i]] = i + 1

    # Greedy dựng chỉ số ký tự c[1..n] (0 = 'a'): suffix nhỏ nhất SA[0] nhận 'a',
    # rồi duyệt các suffix liên tiếp theo thứ tự SA.
    c = [0] * (n + 1)
    cur = 0
    c[sa[0]] = 0
    for i in range(1, n):
        p = sa[i - 1]
        q = sa[i]
        if rank[p + 1] < rank[q + 1]:
            # Phần đuôi đã đúng thứ tự -> giữ nguyên ký tự để dùng ít ký tự nhất.
            c[q] = cur
        else:
            # Đuôi ngược thứ tự -> bắt buộc tăng ký tự.
            cur += 1
            c[q] = cur

    # Cần quá 26 ký tự phân biệt thì không biểu diễn được bằng a-z.
    if cur > 25:
        sys.stdout.write("-1\n")
        return

    # Ghép các ký tự theo vị trí 1..n thành xâu kết quả.
    res = bytes(97 + c[i] for i in range(1, n + 1))
    sys.stdout.write(res.decode() + "\n")


main()
