import sys


def solve(n, t, k):
    # cap = min(k)*t luôn là cận trên hợp lệ: riêng máy nhanh nhất làm xong t
    # sản phẩm trong min(k)*t giây, nên f(min(k)*t) >= t.
    cap = min(k) * t

    # S = tổng(1/k_i). Với số học chính xác: T*S - n < f(T) = sum(T // k_i) <= T*S,
    # nên đáp án nằm trong [t/S, (t+n)/S] - cửa sổ rộng ~n/S -> chỉ khoảng 30 vòng
    # chặt nhị phân. S tính bằng số thực nên chỉ dùng như gợi ý nhanh; các chốt kiểm
    # tra bằng số nguyên bên dưới bảo đảm khoảng tìm kiếm luôn đúng dù double có sai số.
    S = 0.0
    for x in k:
        S += 1.0 / x

    # Nới rộng tương đối 1e-6 để hấp thụ sai số dấu phẩy động của S (với n tới 2*10^5,
    # sai số này nhỏ hơn nhiều bậc), giữ khoảng gợi ý vẫn chặt trong trường hợp thường.
    lo = int(t / S * (1.0 - 1e-6)) - 5
    if lo < 1:
        lo = 1
    hi = int((t + n) / S * (1.0 + 1e-6)) + 5
    if hi > cap or hi < 1:
        hi = cap

    # Chốt kiểm tra bằng số nguyên chính xác: bảo đảm đáp án thật A (giá trị T nhỏ
    # nhất với f(T) >= t) thỏa lo <= A <= hi, nên tính đúng không phụ thuộc gợi ý thực.
    #   * A <= hi  <=>  f(hi) >= t.   Nếu không, nới hi về cap chắc chắn hợp lệ.
    #   * A >= lo  <=>  f(lo-1) < t.  Nếu không, lo đã vượt đáp án; đặt lại về 1.
    if sum(map(hi.__floordiv__, k)) < t:
        hi = cap
    if lo > 1 and sum(map((lo - 1).__floordiv__, k)) >= t:
        lo = 1
    if lo > hi:            # dự phòng an toàn số học: khoảng luôn hợp lệ
        lo = 1

    # Chặt nhị phân tìm T nhỏ nhất với sum(T // k_i) >= t.
    # sum(map(mid.__floordiv__, k)) chạy toàn bộ vòng chia lấy nguyên ở tầng C.
    while lo < hi:
        mid = (lo + hi) >> 1
        if sum(map(mid.__floordiv__, k)) >= t:
            hi = mid
        else:
            lo = mid + 1

    return lo


def main():
    # Đọc nhanh toàn bộ dữ liệu vào; ghi kết quả bằng sys.stdout.write
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    t = int(data[1])
    k = list(map(int, data[2:2 + n]))
    sys.stdout.write(str(solve(n, t, k)) + "\n")


if __name__ == "__main__":
    main()
