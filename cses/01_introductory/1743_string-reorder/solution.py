import sys


def main():
    data = sys.stdin.buffer.read().split()
    w = sys.stdout.buffer.write
    if not data:
        w(b"\n")
        return
    s = data[0]
    n = len(s)

    # Đếm số lần xuất hiện của từng chữ cái A-Z.
    count = [0] * 26
    for ch in s:
        count[ch - 65] += 1

    # Điều kiện tồn tại: sắp xếp được khi và chỉ khi 2*curmax <= n+1.
    curmax = max(count)
    if 2 * curmax > n + 1:
        w(b"-1\n")
        return

    # freq[k] = số chữ cái đang còn đúng k lần; giúp cập nhật curmax O(1)/bước.
    freq = [0] * (curmax + 2)
    for i in range(26):
        freq[count[i]] += 1

    # active = danh sách (đã sắp xếp tăng dần) các chữ cái còn count > 0.
    active = [i for i in range(26) if count[i] > 0]

    # maxletter = một chữ cái đạt count == curmax (kiểm chứng lại khi cần).
    maxletter = 0
    for i in range(26):
        if count[i] == curmax:
            maxletter = i
            break

    res = bytearray(n)
    prev = -1  # chữ cái vừa đặt (-1 = chưa có)
    m = n      # số ký tự còn phải đặt

    # Tham lam theo từng vị trí: đặt chữ nhỏ nhất mà phần còn lại vẫn khả thi.
    for pos in range(n):
        if 2 * curmax > m:
            # Trạng thái CĂNG (2*curmax == m+1, m lẻ): chữ trội là duy nhất
            # và BUỘC phải đặt ngay, nếu không sẽ không xếp vừa nữa.
            if count[maxletter] != curmax:
                # maxletter đã "cũ" -> quét lại tìm chữ đang đạt curmax.
                for i in active:
                    if count[i] == curmax:
                        maxletter = i
                        break
            c = maxletter
        else:
            # Trạng thái LỎNG: chọn chữ nhỏ nhất khác chữ liền trước (prev).
            c = active[0]
            if c == prev:
                c = active[1]

        res[pos] = 65 + c

        # Giảm 1 lần dùng chữ đã chọn và cập nhật các cấu trúc phụ.
        v = count[c]
        count[c] = v - 1
        freq[v] -= 1
        freq[v - 1] += 1
        if v == curmax and freq[curmax] == 0:
            curmax -= 1  # không còn chữ nào đạt curmax -> giảm 1.
        if v == 1:
            active.remove(c)  # hết chữ này.
        prev = c
        m -= 1

    w(bytes(res))
    w(b"\n")


main()
