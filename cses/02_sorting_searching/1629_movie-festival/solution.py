import sys


def main():
    # Đọc toàn bộ dữ liệu một lần rồi tách số (fast I/O).
    data = sys.stdin.buffer.read().split()
    if not data:
        return

    idx = 0
    n = int(data[idx]); idx += 1

    movies = []
    for _ in range(n):
        a = int(data[idx]); idx += 1
        b = int(data[idx]); idx += 1
        # Lưu (end, start) để sắp xếp tự nhiên theo thời điểm kết thúc.
        movies.append((b, a))

    # Sắp xếp tăng dần theo thời điểm kết thúc (greedy).
    movies.sort()

    count = 0
    cur_end = -1  # Thời điểm kết thúc của phim vừa chọn gần nhất.
    for end, start in movies:
        # Chọn phim nếu nó bắt đầu không sớm hơn lúc phim trước kết thúc.
        if start >= cur_end:
            count += 1
            cur_end = end

    sys.stdout.write(str(count) + "\n")


if __name__ == "__main__":
    main()
