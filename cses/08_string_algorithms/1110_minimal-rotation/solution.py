import sys


def least_rotation(S):
    """Booth's algorithm: trả về chỉ số bắt đầu của rotation nhỏ nhất.

    S là xâu đã NHÂN ĐÔI (s + s), làm việc trên bytes (mỗi phần tử là int).
    """
    n2 = len(S)                 # = 2 * n
    f = [-1] * n2               # failure function (tương tự KMP)
    k = 0                       # vị trí bắt đầu rotation nhỏ nhất tìm được
    for j in range(1, n2):
        sj = S[j]
        i = f[j - k - 1]
        # So sánh với ứng viên bắt đầu từ k, dùng failure để nhảy lùi khi lệch;
        # gặp ký tự nhỏ hơn thì dời điểm bắt đầu k sang rotation tốt hơn.
        while i != -1 and sj != S[k + i + 1]:
            if sj < S[k + i + 1]:
                k = j - i - 1
            i = f[i]
        # Cập nhật failure function cho vị trí lệch so với điểm bắt đầu k.
        if sj != S[k + i + 1]:      # tức là i == -1
            if sj < S[k + i + 1]:   # khi i == -1 thì S[k+i+1] == S[k]
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
    doubled = s + s             # nhân đôi để tránh chỉ số modulo
    k = least_rotation(doubled)
    # Rotation nhỏ nhất là đoạn độ dài n bắt đầu tại k.
    out = doubled[k:k + n]
    sys.stdout.buffer.write(out)
    sys.stdout.buffer.write(b"\n")


if __name__ == "__main__":
    main()
