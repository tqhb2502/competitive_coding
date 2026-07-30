import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1

    # Đọc n cặp (thời gian a, deadline d).
    tasks = []
    for _ in range(n):
        a = int(data[idx]); d = int(data[idx + 1]); idx += 2
        tasks.append((a, d))

    # Greedy Shortest Processing Time: sắp xếp theo thời gian a tăng dần
    # để tối thiểu tổng các thời điểm kết thúc, nhờ đó tối đa tổng phần thưởng.
    tasks.sort(key=lambda x: x[0])

    # Duyệt theo thứ tự đã sắp: t là thời điểm kết thúc hiện tại,
    # cộng dồn phần thưởng (d - t) của từng công việc.
    t = 0
    ans = 0
    for a, d in tasks:
        t += a
        ans += d - t

    sys.stdout.write(str(ans) + "\n")


if __name__ == "__main__":
    main()
