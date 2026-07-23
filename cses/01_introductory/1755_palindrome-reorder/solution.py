import sys


def main():
    # Đọc I/O nhanh: đọc toàn bộ input dạng bytes rồi tách lấy xâu đầu tiên.
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    s = data[0]  # kiểu bytes, các ký tự A-Z

    # Đếm số lần xuất hiện của từng ký tự in hoa (ord('A') == 65).
    counts = [0] * 26
    for b in s:
        counts[b - 65] += 1

    # Tìm ký tự có tần suất lẻ (nếu có) và đếm số ký tự tần suất lẻ.
    odd_index = -1
    odd_count = 0
    for i in range(26):
        if counts[i] & 1:
            odd_count += 1
            odd_index = i

    # Có nhiều hơn 1 ký tự tần suất lẻ thì không thể ghép đôi -> vô nghiệm.
    if odd_count > 1:
        sys.stdout.write("NO SOLUTION\n")
        return

    # Dựng nửa trái: mỗi ký tự lấy (counts[i] // 2) bản, theo thứ tự A->Z.
    half_parts = []
    for i in range(26):
        if counts[i] >= 2:
            half_parts.append(chr(65 + i) * (counts[i] // 2))
    left = "".join(half_parts)

    # Ký tự giữa là ký tự tần suất lẻ (nếu có), ngược lại để rỗng.
    middle = chr(65 + odd_index) if odd_index != -1 else ""

    # Kết quả = nửa trái + ký tự giữa + nửa trái đảo ngược.
    result = left + middle + left[::-1]
    sys.stdout.write(result)
    sys.stdout.write("\n")


main()
