import sys

def build_lps(p):
    m = len(p)
    lps = [0] * m

    j = 0
    for i in range(1, m):
        while j > 0 and p[i] != p[j]:
            j = lps[j - 1]

        if p[i] == p[j]:
            j += 1
            lps[i] = j

    return lps


def main():
    input = sys.stdin.buffer.readline

    s = input().strip()
    p = input().strip()

    n = len(s)
    m = len(p)

    if m > n:
        print(0)
        return

    lps = build_lps(p)

    ans = 0
    j = 0

    for c in s:
        while j > 0 and c != p[j]:
            j = lps[j - 1]

        if c == p[j]:
            j += 1

        if j == m:
            ans += 1
            j = lps[j - 1]

    print(ans)


if __name__ == "__main__":
    main()
