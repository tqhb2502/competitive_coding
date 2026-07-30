import sys
import heapq


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1

    # Gắn thêm chỉ số gốc i để cuối cùng in lại đúng thứ tự đầu vào
    customers = [None] * n
    for i in range(n):
        a = int(data[idx]); b = int(data[idx + 1]); idx += 2
        customers[i] = (a, b, i)

    # Sắp xếp khách theo ngày đến a tăng dần
    customers.sort()

    ans = [0] * n          # số phòng gán cho từng khách (theo chỉ số gốc)
    heap = []              # min-heap (ngày_đi, số_phòng) của các phòng đang dùng
    total_rooms = 0        # tổng số phòng đã mở (= đáp án k)

    for a, b, orig in customers:
        if heap and heap[0][0] < a:
            # Phòng có ngày đi nhỏ nhất đã trống trước khi khách này đến -> tái dùng
            dep, room = heapq.heappop(heap)
            ans[orig] = room
            heapq.heappush(heap, (b, room))
        else:
            # Không phòng nào kịp trống -> mở phòng mới
            total_rooms += 1
            room = total_rooms
            ans[orig] = room
            heapq.heappush(heap, (b, room))

    out = [str(total_rooms), " ".join(map(str, ans))]
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
