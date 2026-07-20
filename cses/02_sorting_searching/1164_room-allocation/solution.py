# Room Allocation - CSES 1164
# https://cses.fi/problemset/task/1164
# Greedy + min-heap: sắp xếp theo ngày đến, tái sử dụng phòng có ngày đi nhỏ nhất.

import sys
import heapq


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1

    customers = [None] * n
    for i in range(n):
        a = int(data[idx]); b = int(data[idx + 1]); idx += 2
        customers[i] = (a, b, i)

    # Sắp xếp theo ngày đến a tăng dần
    customers.sort()

    ans = [0] * n          # số phòng cho từng khách (theo chỉ số gốc)
    heap = []              # (ngày_đi, số_phòng) của các phòng đang sử dụng
    total_rooms = 0        # số phòng đã mở (= đáp án k)

    for a, b, orig in customers:
        if heap and heap[0][0] < a:
            # Phòng có ngày đi nhỏ nhất đã trống -> tái sử dụng
            dep, room = heapq.heappop(heap)
            ans[orig] = room
            heapq.heappush(heap, (b, room))
        else:
            # Mở phòng mới
            total_rooms += 1
            room = total_rooms
            ans[orig] = room
            heapq.heappush(heap, (b, room))

    out = [str(total_rooms), " ".join(map(str, ans))]
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
