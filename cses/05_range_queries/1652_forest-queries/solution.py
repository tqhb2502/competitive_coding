import sys


def main():
    # Đọc toàn bộ input một lần cho nhanh; mỗi hàng lưới là một token bytes.
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    q = int(data[idx]); idx += 1

    STAR = 42  # mã ord('*'), so sánh trực tiếp trên bytes cho nhanh

    # Bảng cộng dồn hai chiều: P[i] là hàng prefix-sum độ dài n+1 (1-indexed).
    # P[0] toàn số 0 để làm biên cho công thức bù trừ.
    P = [[0] * (n + 1)]
    prev = P[0]
    for _ in range(n):
        row = data[idx]; idx += 1
        cur = [0] * (n + 1)
        s = 0  # cộng dồn số cây theo cột trong hàng hiện tại
        for j in range(n):
            if row[j] == STAR:
                s += 1
            # P[i][j] = P[i-1][j] + (cộng dồn theo cột trong hàng i)
            cur[j + 1] = prev[j + 1] + s
        P.append(cur)
        prev = cur

    out = []
    ap = out.append
    for _ in range(q):
        y1 = int(data[idx]); x1 = int(data[idx + 1])
        y2 = int(data[idx + 2]); x2 = int(data[idx + 3])
        idx += 4
        # Trả lời truy vấn O(1) bằng nguyên lý bù trừ (inclusion-exclusion).
        ap(P[y2][x2] - P[y1 - 1][x2] - P[y2][x1 - 1] + P[y1 - 1][x1 - 1])

    sys.stdout.buffer.write(('\n'.join(map(str, out)) + '\n').encode())


main()
