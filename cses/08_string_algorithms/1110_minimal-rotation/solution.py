# Minimal Rotation - CSES 1110
# https://cses.fi/problemset/task/1110
# Booth's algorithm: tim lexicographically minimal rotation trong O(n),
# deterministic (khong dung hashing nen an toan voi anti-hash tests).

import sys


def least_rotation(S):
    """Booth's algorithm. Tra ve chi so bat dau cua rotation nho nhat.

    S la xau da NHAN DOI (s + s). Ham lam viec tren bytes (moi phan tu la int).
    """
    n2 = len(S)                 # = 2 * n
    f = [-1] * n2               # failure function
    k = 0                       # vi tri bat dau rotation nho nhat tim duoc
    for j in range(1, n2):
        sj = S[j]
        i = f[j - k - 1]
        while i != -1 and sj != S[k + i + 1]:
            if sj < S[k + i + 1]:
                k = j - i - 1
            i = f[i]
        if sj != S[k + i + 1]:      # tuc la i == -1
            if sj < S[k + i + 1]:   # khi i == -1 thi S[k+i+1] == S[k]
                k = j
            f[j - k] = -1
        else:
            f[j - k] = i + 1
    return k


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    s = data[0]                 # bytes
    n = len(s)
    doubled = s + s             # nhan doi de tranh chi so modulo
    k = least_rotation(doubled)
    out = doubled[k:k + n]
    sys.stdout.buffer.write(out)
    sys.stdout.buffer.write(b"\n")


if __name__ == "__main__":
    main()
