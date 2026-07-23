import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    s = data[0]  # làm việc trực tiếp trên bytes (mỗi phần tử là một int)
    n = len(s)

    # Z-function: duy trì cửa sổ [l, r) là đoạn khớp prefix xa nhất về bên phải.
    # z[0] = 0 theo quy ước z(1)=0 của đề.
    z = [0] * n
    l = 0
    r = 0
    for i in range(1, n):
        zi = 0
        # Nếu còn nằm trong cửa sổ thì tận dụng giá trị đã tính trước
        if i < r:
            d = r - i
            zli = z[i - l]
            zi = d if d < zli else zli
        # Mở rộng bằng cách so sánh trực tiếp với prefix
        while i + zi < n and s[zi] == s[i + zi]:
            zi += 1
        z[i] = zi
        # Cập nhật cửa sổ khi khớp được xa hơn
        if i + zi > r:
            l = i
            r = i + zi

    # Prefix function (KMP): border dài nhất vừa là prefix vừa là suffix
    pi = [0] * n
    for i in range(1, n):
        j = pi[i - 1]
        si = s[i]
        # Quay lui theo chuỗi border khi ký tự không khớp
        while j > 0 and si != s[j]:
            j = pi[j - 1]
        if si == s[j]:
            j += 1
        pi[i] = j

    # Dòng 1: Z-function, dòng 2: prefix function; gộp thành một lần ghi
    out = sys.stdout
    out.write(' '.join(map(str, z)))
    out.write('\n')
    out.write(' '.join(map(str, pi)))
    out.write('\n')


main()
