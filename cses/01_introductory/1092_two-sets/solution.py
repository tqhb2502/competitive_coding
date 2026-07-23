import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])

    # Tổng 1..n = n*(n+1)/2 chỉ chẵn khi n % 4 == 0 hoặc n % 4 == 3; ngoài ra không chia được.
    if n % 4 != 0 and n % 4 != 3:
        sys.stdout.write("NO\n")
        return

    target = n * (n + 1) // 4  # tổng mà mỗi tập cần đạt được

    set1 = []
    set2 = []
    remaining = target
    # Tham lam: duyệt số lớn nhất về nhỏ nhất, lấy i vào tập 1 khi còn "vừa" (i <= remaining),
    # phần còn lại dồn sang tập 2 nên tổng tập 2 cũng bằng target.
    for i in range(n, 0, -1):
        if i <= remaining:
            set1.append(i)
            remaining -= i
        else:
            set2.append(i)

    # Gom kết quả rồi xuất một lần để tối ưu I/O.
    out = []
    out.append("YES")
    out.append(str(len(set1)))
    out.append(" ".join(map(str, set1)))
    out.append(str(len(set2)))
    out.append(" ".join(map(str, set2)))
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
