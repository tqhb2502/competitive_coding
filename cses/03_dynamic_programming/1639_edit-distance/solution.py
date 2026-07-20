# Edit Distance - CSES 1639
# https://cses.fi/problemset/task/1639
#
# Levenshtein edit distance giữa hai chuỗi.
# Dùng thuật toán bit-parallel của Myers/Hyyrö: biểu diễn các delta dọc
# giữa các ô kề nhau bằng bitmask VP/VN (số nguyên lớn m bit), cập nhật theo
# từng ký tự của text bằng các phép bitwise + một phép cộng tạo carry.
# Khác bản approximate-matching: hàng 0 của bảng DP là 0,1,2,... nên khi dịch
# trái HP ta nhồi bit 1 vào ((Ph<<1)|1). Độ phức tạp ~ O(n*m/64), rất nhanh.

import sys


def edit_distance(P, T):
    # P là pattern (rộng m bit), T là text (duyệt n vòng).
    m = len(P)
    if m == 0:
        return len(T)
    Peq = {}
    for i, ch in enumerate(P):
        Peq[ch] = Peq.get(ch, 0) | (1 << i)
    mask = (1 << m) - 1
    hb = 1 << (m - 1)          # bit cao nhất = hàng m (dp[m][.])
    VP = mask                   # cột 0: dp[i][0]-dp[i-1][0] = +1 với mỗi i
    VN = 0
    score = m                   # dp[m][0] = m
    Peq_get = Peq.get
    for ch in T:
        Eq = Peq_get(ch, 0)
        Xv = Eq | VN
        Xh = (((Eq & VP) + VP) ^ VP) | Eq   # D0: cho biết ô lấy từ đường chéo
        Ph = VN | ~(Xh | VP)                # delta ngang dương (+1)
        Mh = VP & Xh                        # delta ngang âm (-1)
        Ph &= mask
        Mh &= mask
        if Ph & hb:
            score += 1
        elif Mh & hb:
            score -= 1
        Ph = ((Ph << 1) | 1) & mask         # nhồi +1 cho hàng 0 (edit distance toàn cục)
        Mh = (Mh << 1) & mask
        VP = (Mh | ~(Xv | Ph)) & mask
        VN = (Ph & Xv) & mask
    return score


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        print(0)
        return
    s = data[0].decode()
    t = data[1].decode() if len(data) > 1 else ""
    # Chọn chuỗi ngắn hơn làm pattern để số nguyên lớn nhỏ lại.
    if len(s) <= len(t):
        ans = edit_distance(s, t)
    else:
        ans = edit_distance(t, s)
    sys.stdout.write(str(ans) + "\n")


main()
