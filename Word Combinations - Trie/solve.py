import sys

MOD = 10**9 + 7

def main():
    input = sys.stdin.buffer.readline

    s = input().strip()
    n = len(s)

    k = int(input())

    # Trie dạng mảng phẳng:
    # next[node * 26 + c] = node con, hoặc -1 nếu không có
    nxt = [-1] * 26
    terminal = bytearray([0])

    nodes = 1
    empty_node = [-1] * 26

    for _ in range(k):
        w = input().strip()

        # Từ dài hơn chuỗi chính thì chắc chắn không dùng được
        if len(w) > n:
            continue

        node = 0
        base = 0

        for ch in w:
            c = ch - 97
            idx = base + c

            child = nxt[idx]

            if child == -1:
                child = nodes
                nodes += 1

                nxt[idx] = child
                nxt.extend(empty_node)
                terminal.append(0)

            node = child
            base = node * 26

        terminal[node] = 1

    s_arr = [ch - 97 for ch in s]

    dp = [0] * (n + 1)
    dp[n] = 1

    for i in range(n - 1, -1, -1):
        node = 0
        base = 0
        total = 0

        for j in range(i, n):
            child = nxt[base + s_arr[j]]

            if child == -1:
                break

            node = child

            if terminal[node]:
                total += dp[j + 1]
                if total >= MOD:
                    total -= MOD

            base = node * 26

        dp[i] = total

    print(dp[0])


if __name__ == "__main__":
    main()
