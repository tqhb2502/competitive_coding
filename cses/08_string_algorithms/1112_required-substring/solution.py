import sys

MOD = 10**9 + 7


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    pat = data[1] if len(data) > 1 else b""
    m = len(pat)

    # Mã hóa pattern về khoảng 0..25 (A = 0).
    P = [b - 65 for b in pat]

    # Prefix function (failure function) của KMP.
    pi = [0] * m
    for i in range(1, m):
        k = pi[i - 1]
        while k > 0 and P[i] != P[k]:
            k = pi[k - 1]
        if P[i] == P[k]:
            k += 1
        pi[i] = k

    # Xây KMP automaton: aut[j][c] = trạng thái kế tiếp từ trạng thái j khi đọc
    # ký tự c. j chạy 0..m-1 (chưa khớp hết); nếu aut[j][c] == m tức pattern
    # đã xuất hiện đầy đủ.
    aut = [[0] * 26 for _ in range(m)]
    for j in range(m):
        pj = P[j]
        row = aut[j]
        if j == 0:
            for c in range(26):
                row[c] = 1 if c == pj else 0
        else:
            # Không khớp thì nhảy theo failure link pi[j-1].
            prev = aut[pi[j - 1]]
            for c in range(26):
                row[c] = j + 1 if c == pj else prev[c]

    # Nén chuyển tiếp: với mỗi trạng thái j, gom các ký tự dẫn tới cùng một
    # trạng thái < m (bỏ ký tự dẫn tới m vì các xâu đó đã chứa pattern).
    trans = []
    for j in range(m):
        cnt = {}
        row = aut[j]
        for c in range(26):
            nx = row[c]
            if nx < m:
                cnt[nx] = cnt.get(nx, 0) + 1
        trans.append(list(cnt.items()))

    # DP: dp[j] = số xâu độ dài hiện tại, chưa bao giờ đạt trạng thái m, kết
    # thúc ở trạng thái j. Khởi tạo dp[0] = 1 cho xâu rỗng.
    dp = [0] * m
    dp[0] = 1
    for _ in range(n):
        ndp = [0] * m
        for j in range(m):
            v = dp[j]
            if v:
                for nx, k in trans[j]:
                    ndp[nx] = (ndp[nx] + v * k) % MOD
        dp = ndp

    # Tổng dp là số xâu không chứa pattern; kết quả = 26^n trừ đi số đó.
    not_contain = sum(dp) % MOD
    ans = (pow(26, n, MOD) - not_contain) % MOD
    sys.stdout.write(str(ans) + "\n")


main()
