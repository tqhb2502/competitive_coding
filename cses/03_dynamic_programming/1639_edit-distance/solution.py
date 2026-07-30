import sys


def edit_distance(P, T):
    # P là pattern (rộng m bit), T là text (duyệt n vòng).
    m = len(P)
    if m == 0:
        return len(T)

    # Peq[ch] = bitmask các vị trí ký tự ch xuất hiện trong pattern P.
    Peq = {}
    for i, ch in enumerate(P):
        Peq[ch] = Peq.get(ch, 0) | (1 << i)

    mask = (1 << m) - 1
    hb = 1 << (m - 1)           # bit cao nhất ứng với hàng m (dp[m][.])
    VP = mask                   # cột 0: dp[i][0]-dp[i-1][0] = +1 với mỗi i
    VN = 0
    score = m                   # dp[m][0] = m
    Peq_get = Peq.get

    for ch in T:
        Eq = Peq_get(ch, 0)
        Xv = Eq | VN
        Xh = (((Eq & VP) + VP) ^ VP) | Eq   # D0: ô lấy giá trị từ đường chéo
        Ph = VN | ~(Xh | VP)                # delta ngang dương (+1)
        Mh = VP & Xh                        # delta ngang âm (-1)
        Ph &= mask
        Mh &= mask
        # Cập nhật score theo delta ngang tại bit cao nhất (hàng m).
        if Ph & hb:
            score += 1
        elif Mh & hb:
            score -= 1
        # Dịch trái và nhồi +1 cho hàng 0 (đặc thù edit distance toàn cục).
        Ph = ((Ph << 1) | 1) & mask
        Mh = (Mh << 1) & mask
        # Cập nhật delta dọc VP/VN cho vòng ký tự kế tiếp.
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
