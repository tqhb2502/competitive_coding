# Word Combinations - CSES 1731
# https://cses.fi/problemset/task/1731
# Trie cua tu dien + DP: dp[i] = so cach tao thanh tien to s[0..i-1].
# Voi moi vi tri i, di tren Trie theo cac ky tu s[i..] va cong dp[i] vao dp[j]
# moi khi gap mot nut ket thuc mot tu (s[i..j-1] la mot tu trong tu dien).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        sys.stdout.write("0\n")
        return

    s = data[0]                       # bytes; s[j] cho ra gia tri byte (int)
    k = int(data[1]) if len(data) > 1 else 0
    words = data[2:2 + k]
    n = len(s)
    MOD = 1000000007

    # Trie luu bang mot dict chung: key = (node << 7) | char -> chi so nut con.
    # is_word[v] = 1 neu nut v la ket thuc cua mot tu.
    trie = {}
    is_word = bytearray()
    is_word.append(0)                 # nut goc co chi so 0
    node_count = 1
    for w in words:
        if len(w) > n:                # tu dai hon s khong the khop -> bo qua
            continue
        v = 0
        for c in w:
            key = (v << 7) | c
            nxt = trie.get(key)
            if nxt is None:
                nxt = node_count
                node_count += 1
                trie[key] = nxt
                is_word.append(0)
            v = nxt
        is_word[v] = 1

    dp = [0] * (n + 1)
    dp[0] = 1

    trie_get = trie.get               # ham cuc bo cho toc do
    isw = is_word
    ss = s
    for i in range(n):
        di = dp[i]
        if not di:
            continue
        di %= MOD
        v = 0
        j = i
        while j < n:
            nxt = trie_get((v << 7) | ss[j])
            if nxt is None:
                break
            v = nxt
            if isw[v]:
                dp[j + 1] += di       # di < MOD, cong don an toan (< ~5e12)
            j += 1

    sys.stdout.write(str(dp[n] % MOD) + "\n")


main()
