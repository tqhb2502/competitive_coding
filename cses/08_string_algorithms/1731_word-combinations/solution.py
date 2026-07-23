import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        sys.stdout.write("0\n")
        return

    s = data[0]                       # dạng bytes; s[j] trả về giá trị byte (int)
    k = int(data[1]) if len(data) > 1 else 0
    words = data[2:2 + k]
    n = len(s)
    MOD = 1000000007

    # Lưu Trie trong một dict chung: khóa = (node << 7) | char -> chỉ số nút con.
    # is_word[v] = 1 nếu nút v là điểm kết thúc của một từ trong từ điển.
    trie = {}
    is_word = bytearray()
    is_word.append(0)                 # nút gốc mang chỉ số 0
    node_count = 1
    for w in words:
        if len(w) > n:                # từ dài hơn s không bao giờ khớp -> bỏ qua
            continue
        v = 0
        for c in w:
            key = (v << 7) | c
            nxt = trie.get(key)
            if nxt is None:           # tạo nút con mới nếu chưa tồn tại
                nxt = node_count
                node_count += 1
                trie[key] = nxt
                is_word.append(0)
            v = nxt
        is_word[v] = 1                # đánh dấu kết thúc từ

    # dp[i] = số cách ghép được tiền tố s[0..i-1]; dp[0] = 1 (chuỗi rỗng).
    dp = [0] * (n + 1)
    dp[0] = 1

    trie_get = trie.get               # gán cục bộ để tăng tốc
    isw = is_word
    ss = s
    for i in range(n):
        di = dp[i]
        if not di:
            continue
        di %= MOD
        # Đi trên Trie theo các ký tự s[i..]; mỗi khi gặp nút kết thúc từ
        # (tức s[i..j] là một từ) thì cộng dp[i] vào dp[j+1].
        v = 0
        j = i
        while j < n:
            nxt = trie_get((v << 7) | ss[j])
            if nxt is None:           # không còn nhánh khớp -> dừng
                break
            v = nxt
            if isw[v]:
                dp[j + 1] += di       # di < MOD nên cộng dồn an toàn (< ~5e12)
            j += 1

    sys.stdout.write(str(dp[n] % MOD) + "\n")


main()
