import sys


def main():
    # P-position cuối cùng của trò chơi là 1222; chọn ngưỡng B lớn hơn một chút.
    B = 1300
    # Tính giá trị Grundy g(k) theo công thức truy hồi Sprague-Grundy.
    # g[1] = g[2] = 0 vì không thể tách thành hai đống khác kích thước.
    g = [0] * (B + 1)
    for k in range(3, B + 1):
        m = (k - 1) // 2  # a chạy 1..m để bảo đảm a < k-a (hai đống khác nhau)
        # Tập các nimber đạt được khi tách đống k thành a và k-a.
        seen = set()
        add = seen.add
        for a in range(1, m + 1):
            add(g[a] ^ g[k - a])
        # g(k) = mex của seen (số nguyên không âm nhỏ nhất chưa xuất hiện).
        mex = 0
        while mex in seen:
            mex += 1
        g[k] = mex

    # Đọc toàn bộ input một lần cho nhanh.
    data = sys.stdin.buffer.read().split()
    t = int(data[0])
    out = []
    for i in range(1, t + 1):
        n = int(data[i])
        if n <= B:
            # g(n)==0 là P-position: người đi thua -> "second"; ngược lại "first".
            out.append("second" if g[n] == 0 else "first")
        else:
            out.append("first")  # vượt 1222 thì không còn P-position nào
    sys.stdout.write("\n".join(out))
    if out:
        sys.stdout.write("\n")


main()
