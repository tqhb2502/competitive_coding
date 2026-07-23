import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    t = int(data[0])
    idx = 1
    out = []
    for _ in range(t):
        n = int(data[idx]); a = int(data[idx + 1]); b = int(data[idx + 2])
        idx += 3

        # Điều kiện khả thi:
        #  - a + b <= n (phần còn lại là các ván hòa);
        #  - a == 0 khi và chỉ khi b == 0 (không thể có excedance mà thiếu deficiency).
        if a + b > n or (a == 0) != (b == 0):
            out.append("NO")
            continue

        # m lá bài đầu tham gia phân thắng thua; n - m lá còn lại đều hòa.
        m = a + b
        # Người 2 (điểm b) đánh theo thứ tự 1..n.
        p2 = list(range(1, n + 1))
        # Người 1 (điểm a): dịch vòng tròn theo b trên m lá đầu -> đúng a ván thắng, b ván thua.
        # [b+1, b+2, ..., m, 1, 2, ..., b] rồi đến các lá hòa m+1..n.
        p1 = list(range(b + 1, m + 1)) + list(range(1, b + 1)) + list(range(m + 1, n + 1))

        out.append("YES")
        out.append(" ".join(map(str, p1)))
        out.append(" ".join(map(str, p2)))

    sys.stdout.write("\n".join(out) + "\n")


main()
