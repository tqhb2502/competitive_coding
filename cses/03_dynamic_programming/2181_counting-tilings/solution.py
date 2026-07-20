# Counting Tilings - https://cses.fi/problemset/task/2181
# Đếm số cách lát kín lưới n x m bằng các quân domino 1x2 và 2x1, lấy modulo 1e9+7.
# Phương pháp: profile DP theo từng cột, bitmask trên n hàng (n <= 10 nên 2^n <= 1024).
# Tối ưu cho CPython: sinh sẵn một hàm cộng dồn (code-gen) nhằm đưa toàn bộ vòng lặp
# cộng bên trong xuống bytecode, tránh chi phí vòng lặp Python cấp cao.
import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    m = int(data[1])
    MOD = 10 ** 9 + 7

    # Số cách lát lưới (n x m) bằng số cách lát (m x n) do đối xứng chuyển vị
    # (domino ngang đổi thành dọc). Chọn n là chiều nhỏ hơn để 2^n nhỏ nhất.
    if n > m:
        n, m = m, n

    size = 1 << n  # số trạng thái bitmask trên n hàng

    # incoming[nxt] = danh sách các mask cur có thể sinh ra nxt.
    # cur = tập các ô của cột hiện tại đã bị một quân domino ngang từ cột trước "thò"
    # sang chiếm chỗ. Ta lát đầy phần còn lại của cột bằng domino dọc (trong cột)
    # và domino ngang (thò sang cột kế); nxt = tập các ô bị thò sang cột kế tiếp.
    incoming = [[] for _ in range(size)]
    for cur in range(size):
        # duyệt DFS lặp: điền các ô từ hàng 0 đến hàng n-1.
        stack = [(0, 0)]  # (hàng đang xét, mask các ô thò sang cột kế)
        while stack:
            row, nxt = stack.pop()
            if row == n:
                incoming[nxt].append(cur)
                continue
            if (cur >> row) & 1:
                # ô này đã bị chiếm từ cột trước -> bỏ qua, không thò sang cột kế.
                stack.append((row + 1, nxt))
            else:
                # lựa chọn 1: đặt domino ngang thò sang cột kế -> bật bit row trong nxt.
                stack.append((row + 1, nxt | (1 << row)))
                # lựa chọn 2: đặt domino dọc phủ (row, row+1) ngay trong cột này.
                if row + 1 < n and not ((cur >> (row + 1)) & 1):
                    stack.append((row + 2, nxt))

    # Sinh sẵn hàm step(dp) -> ndp: mỗi phần tử ndp[nxt] là tổng đã biết trước của
    # các dp[cur] (cur thuộc incoming[nxt]), rút gọn modulo. Nhờ hardcode các chỉ số,
    # phép cộng dồn chạy trực tiếp ở tầng bytecode nên rất nhanh trên CPython.
    parts = []
    for nxt in range(size):
        lst = incoming[nxt]
        if lst:
            parts.append("(" + "+".join("d%d" % c for c in lst) + ")%" + str(MOD))
        else:
            parts.append("0")
    src = (
        "def step(dp):\n"
        "    " + ",".join("d%d" % i for i in range(size)) + ",=dp\n"
        "    return [" + ",".join(parts) + "]\n"
    )
    ns = {}
    exec(src, ns)
    step = ns["step"]

    # DP qua m cột. dp[mask] = số cách khi mask là các ô "thừa ra" (bị thò sang) cột đang xét.
    dp = [0] * size
    dp[0] = 1  # trước cột đầu tiên: không có ô nào thò sang.
    for _ in range(m):
        dp = step(dp)

    # Sau m cột không được có ô nào thò ra ngoài lưới -> lấy dp[0].
    print(dp[0] % MOD)


main()
