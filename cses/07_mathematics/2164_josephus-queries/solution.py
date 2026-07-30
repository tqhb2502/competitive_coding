import sys


def solve(n, k):
    # Trả về số hiệu (1-indexed) của đứa trẻ bị loại ở lượt thứ k.
    # Thu nhỏ bài toán theo tầng: trong vòng quét đầu các vị trí chẵn
    # 2,4,...,2*floor(n/2) bị loại. Nếu k rơi vào vòng đầu, đáp án là 2*k.
    # Ngược lại chỉ còn các vị trí lẻ sống sót nên quy về vòng tròn nhỏ hơn,
    # giữ phép biến đổi tuyến tính answer = A + B * (đáp án bài toán con).
    A = 0
    B = 1
    while True:
        m = n // 2                      # số lần loại trong vòng quét đầu
        if k <= m:                      # lượt loại thứ k rơi vào vòng đầu
            return A + B * (2 * k)
        r = k - m                       # chỉ số loại trong vòng nhỏ (các vị trí lẻ)
        if n % 2 == 0:
            # n chẵn: còn lại 1,3,...,n-1 (m đứa), tiếp theo loại 3 => bài toán gốc
            # trên m đứa. Ánh xạ original_pos = 2*sub - 1  ->  A -= B, B *= 2.
            A -= B
            B *= 2
            n = m
            k = r
        else:
            # n lẻ: còn lại 1,3,...,n (m+1 đứa), số 1 bị loại ngay sau vòng đầu.
            if r == 1:
                return A + B            # số 1 (vị trí gốc = 2*1-1 = 1)
            # Sau khi loại số 1, phần còn lại 3,5,...,n thành bài toán gốc trên m đứa.
            # Ánh xạ original_pos = 2*(1 + sub) - 1 = 1 + 2*sub  ->  A += B, B *= 2.
            A += B
            B *= 2
            n = m
            k = r - 1


def main():
    data = sys.stdin.buffer.read().split()
    q = int(data[0])
    out = []
    idx = 1
    for _ in range(q):
        n = int(data[idx]); k = int(data[idx + 1]); idx += 2
        out.append(str(solve(n, k)))
    sys.stdout.write("\n".join(out))
    if out:
        sys.stdout.write("\n")


main()
