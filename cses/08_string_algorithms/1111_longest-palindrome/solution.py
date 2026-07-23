import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    s = data[0]  # bytes, các ký tự a-z (97..122)
    n = len(s)
    if n == 0:
        return

    # Biến đổi xâu với ký tự phân cách '#' (giá trị 0) và hai lính canh khác nhau.
    # Bố cục:  [guard1] # c0 # c1 # ... # c(n-1) # [guard2]
    # Độ dài phần '#' là m = 2n+1; toàn mảng dài m+2. Ký tự c_i nằm ở chỉ số 2i+2.
    m = 2 * n + 1
    t = bytearray(m + 2)          # các ô phân cách vốn đã bằng 0
    t[0] = 1                      # lính canh trái (duy nhất)
    t[m + 1] = 2                  # lính canh phải (duy nhất)
    t[2:2 * n + 1:2] = s          # đặt ký tự gốc vào các vị trí chẵn

    P = [0] * (m + 2)             # bán kính palindrome theo từng tâm

    center = 0
    right = 0
    best_len = 0
    best_center = 1

    tl = t  # gán biến cục bộ để tăng tốc
    Pl = P
    # Manacher: quét mọi tâm i, tính bán kính palindrome p.
    for i in range(1, m + 1):
        # Tận dụng đối xứng qua center để khởi tạo, tránh mở rộng lại từ đầu.
        if i < right:
            mir = 2 * center - i
            r = right - i
            pm = Pl[mir]
            p = pm if pm < r else r
        else:
            p = 0
        # Mở rộng quanh i (guard bảo đảm dừng mà không cần kiểm tra biên).
        while tl[i + p + 1] == tl[i - p - 1]:
            p += 1
        Pl[i] = p
        # Cập nhật palindrome vươn xa nhất (con trỏ right không bao giờ lùi).
        if i + p > right:
            center = i
            right = i + p
        # Lưu tâm cho bán kính lớn nhất (chính là độ dài đáp án trong xâu gốc).
        if p > best_len:
            best_len = p
            best_center = i

    # Suy ngược vị trí bắt đầu trong xâu gốc rồi in palindrome dài nhất.
    start = (best_center - 1 - best_len) // 2
    sys.stdout.buffer.write(s[start:start + best_len] + b"\n")


main()
