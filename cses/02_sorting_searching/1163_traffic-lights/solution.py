import sys


def main():
    # Đọc chiều dài đường x, số đèn n và mảng p theo đúng thứ tự thêm.
    data = sys.stdin.buffer.read().split()
    x = int(data[0])
    n = int(data[1])
    p = [int(data[2 + i]) for i in range(n)]

    # coords = sắp xếp tăng dần của hai biên 0, x cùng toàn bộ vị trí đèn:
    # đây là trạng thái đầy đủ với đủ n đèn.
    coords = sorted(p)
    coords = [0] + coords + [x]
    m = len(coords)  # = n + 2

    # Ánh xạ vị trí đèn -> chỉ số trong coords để biết node cần gỡ.
    pos_to_idx = {}
    for i in range(m):
        pos_to_idx[coords[i]] = i

    # Doubly-linked list trên chỉ số của coords: lấy hàng xóm và gỡ node trong O(1).
    left = list(range(-1, m - 1))   # left[i]  = i - 1
    right = list(range(1, m + 1))   # right[i] = i + 1
    # right[m-1] = m nằm ngoài mảng nhưng không bao giờ dùng (node cuối không bị gỡ).

    # Trạng thái đầy đủ: đoạn trống dài nhất = max hiệu coords liền kề = A[n].
    current_max = 0
    for i in range(1, m):
        d = coords[i] - coords[i - 1]
        if d > current_max:
            current_max = d

    ans = [0] * n
    ans[n - 1] = current_max  # trạng thái có đủ n đèn

    # Xử lý NGƯỢC: gỡ p_n, p_{n-1}, ..., p_2; gỡ p_k cho ra trạng thái S_{k-1}.
    # Mỗi lần gỡ gộp hai đoạn liền kề nên current_max chỉ tăng hoặc giữ nguyên.
    for k in range(n, 1, -1):
        node = pos_to_idx[p[k - 1]]
        L = left[node]
        R = right[node]
        # Gỡ node khỏi linked list, hai hàng xóm nối trực tiếp với nhau.
        right[L] = R
        left[R] = L
        # Đoạn mới gộp lại luôn >= hai đoạn cũ, cập nhật max chạy.
        new_gap = coords[R] - coords[L]
        if new_gap > current_max:
            current_max = new_gap
        ans[k - 2] = current_max  # trạng thái S_{k-1}

    sys.stdout.write(" ".join(map(str, ans)))
    sys.stdout.write("\n")


main()
