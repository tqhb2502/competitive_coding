import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    coins = data[1:1 + n]

    # DP subset-sum bằng bitmask: dùng số nguyên lớn làm bitset,
    # bit thứ i bật nghĩa là tổng i tạo được. Cơ sở: chỉ bit 0 bật (tổng 0)
    dp = 1
    for tok in coins:
        c = int(tok)
        # dp |= dp << c: thêm đồng xu c vào mọi tổng đang có, cập nhật đồng thời
        dp |= dp << c

    # Các bit bật (trừ bit 0) chính là các tổng khả thi, theo thứ tự tăng dần
    bits = bin(dp)[2:][::-1]  # bits[i] là bit thứ i
    sums = [str(i) for i in range(1, len(bits)) if bits[i] == '1']

    # In số lượng tổng khác nhau rồi liệt kê các tổng đó
    out = sys.stdout
    out.write(str(len(sums)))
    out.write("\n")
    out.write(" ".join(sums))
    out.write("\n")


if __name__ == "__main__":
    main()
