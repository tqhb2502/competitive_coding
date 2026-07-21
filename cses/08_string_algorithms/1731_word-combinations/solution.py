# Word Combinations - CSES 1731
# https://cses.fi/problemset/task/1731
# Dictionary trie + DP: dp[i] = number of ways to build the prefix s[0..i-1].
# For every position i, walk the trie following characters s[i..] and add dp[i]
# to dp[j] whenever we hit a node that ends a word (s[i..j-1] is a dictionary word).

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        sys.stdout.write("0\n")
        return

    s = data[0]                       # bytes; s[j] yields the byte value (int)
    k = int(data[1]) if len(data) > 1 else 0
    words = data[2:2 + k]
    n = len(s)
    MOD = 1000000007

    # Trie stored in one shared dict: key = (node << 7) | char -> child node index.
    # is_word[v] = 1 if node v is the end of a word.
    trie = {}
    is_word = bytearray()
    is_word.append(0)                 # root node has index 0
    node_count = 1
    for w in words:
        if len(w) > n:                # a word longer than s can never match -> skip
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

    trie_get = trie.get               # local binding for speed
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
                dp[j + 1] += di       # di < MOD, safe to accumulate (< ~5e12)
            j += 1

    sys.stdout.write(str(dp[n] % MOD) + "\n")


main()
