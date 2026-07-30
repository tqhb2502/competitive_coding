import sys
from bisect import bisect_left


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        print(0)
        return
    n = int(data[0])

    # tails[i] = đuôi nhỏ nhất có thể của dãy con tăng nghiêm ngặt độ dài i+1.
    # Mảng tails luôn tăng nghiêm ngặt nên dùng được binary search.
    tails = []
    for i in range(1, n + 1):
        x = int(data[i])
        # bisect_left: vị trí đầu tiên có giá trị >= x (bảo đảm tăng nghiêm ngặt).
        pos = bisect_left(tails, x)
        if pos == len(tails):
            # x lớn hơn mọi đuôi hiện có -> nối vào cuối, LIS dài thêm 1.
            tails.append(x)
        else:
            # Thay đuôi tốt hơn cho dãy con tăng cùng độ dài.
            tails[pos] = x

    # Đáp án là số cột của tails, tức độ dài LIS.
    sys.stdout.write(str(len(tails)) + "\n")


main()
