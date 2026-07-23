import sys


def main():
    # Đọc toàn bộ đầu vào một lần; giữ phần tử ở dạng chuỗi byte để so sánh nhanh.
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1
    a = data[idx:idx + n]; idx += n
    b = data[idx:idx + m]; idx += m

    # Bảng dp đầy đủ (n+1) x (m+1) để còn truy vết lại dãy con.
    # dp[i][j] = độ dài LCS giữa tiền tố a[0..i-1] và b[0..j-1]; hàng/cột 0 = 0.
    prev = [0] * (m + 1)
    dp = [prev]
    for i in range(n):
        ai = a[i]
        cur = [0] * (m + 1)
        for j in range(m):
            if ai == b[j]:
                # Trùng phần tử cuối: nối vào LCS của hai tiền tố ngắn hơn.
                cur[j + 1] = prev[j] + 1
            else:
                # Khác: lấy max của bỏ phần tử cuối bên a (up) hoặc bên b (left).
                left = cur[j]
                up = prev[j + 1]
                cur[j + 1] = left if left >= up else up
        dp.append(cur)
        prev = cur

    length = dp[n][m]

    # Truy vết ngược từ ô (n, m) để dựng lại một dãy con chung hợp lệ.
    res = []
    i, j = n, m
    while i > 0 and j > 0:
        if a[i - 1] == b[j - 1]:
            # Phần tử này thuộc LCS: ghi lại rồi lùi cả hai chỉ số.
            res.append(a[i - 1])
            i -= 1
            j -= 1
        elif dp[i - 1][j] >= dp[i][j - 1]:
            # Đi về ô có dp lớn hơn.
            i -= 1
        else:
            j -= 1
    # Đảo ngược để có thứ tự đúng từ trái sang phải.
    res.reverse()

    # In độ dài LCS rồi in dãy con.
    out = sys.stdout
    out.write(str(length))
    out.write('\n')
    out.write(b' '.join(res).decode())
    out.write('\n')


main()
