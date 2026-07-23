import sys


def main():
    data = sys.stdin.buffer.read().split()
    nums = list(map(int, data))
    pos = 0
    n = nums[pos]; pos += 1

    X1 = [0] * n
    Y1 = [0] * n
    X2 = [0] * n
    Y2 = [0] * n
    ys_set = set()
    for i in range(n):
        x1 = nums[pos]; y1 = nums[pos + 1]; x2 = nums[pos + 2]; y2 = nums[pos + 3]
        pos += 4
        X1[i] = x1; Y1[i] = y1; X2[i] = x2; Y2[i] = y2
        ys_set.add(y1)
        ys_set.add(y2)

    # Coordinate compression trục y.
    ys = sorted(ys_set)
    yidx = {v: i for i, v in enumerate(ys)}
    M = len(ys) - 1  # số elementary y-interval (>=1 vì luôn có y1<y2)

    # Kích thước segment tree (luỹ thừa 2 >= M).
    sz = 1
    while sz < M:
        sz <<= 1

    # span[node] = tổng chiều dài vật lý theo y của các elementary interval trong node.
    span = [0] * (2 * sz)
    for i in range(M):
        span[sz + i] = ys[i + 1] - ys[i]
    for i in range(sz - 1, 0, -1):
        span[i] = span[2 * i] + span[2 * i + 1]

    # cnt[node]   = số hình chữ nhật active phủ trọn đoạn node (lazy, không push-down).
    # cover[node] = độ dài phần bị phủ trong đoạn node (tính theo cnt tại node và hậu duệ).
    cnt = [0] * (2 * sz)
    cover = [0] * (2 * sz)

    # Dựng các sự kiện dọc: tại x1 cộng +1 trên [y1,y2), tại x2 cộng -1 trên [y1,y2).
    events = []
    ap = events.append
    for i in range(n):
        li = yidx[Y1[i]]
        ri = yidx[Y2[i]]
        ap((X1[i], 1, li, ri))
        ap((X2[i], -1, li, ri))
    events.sort()

    area = 0
    ne = len(events)
    prev_x = events[0][0]
    j = 0
    while j < ne:
        x = events[j][0]
        # Dải [prev_x, x] được phủ với chiều cao = độ dài union hiện tại cover[1].
        area += (x - prev_x) * cover[1]
        # Áp dụng mọi sự kiện có cùng hoành độ x.
        while j < ne and events[j][0] == x:
            _, val, l, r = events[j]
            j += 1
            # Range update +val/-val trên các elementary interval [l, r), duy trì cover
            # bằng công thức cục bộ:
            #   cover = span            nếu cnt > 0
            #   cover = 0               nếu là lá và cnt == 0
            #   cover = con0 + con1     trong các trường hợp còn lại
            l += sz
            r += sz
            ll = l
            rr = r
            while l < r:
                if l & 1:
                    c = cnt[l] + val
                    cnt[l] = c
                    if c > 0:
                        cover[l] = span[l]
                    elif l >= sz:
                        cover[l] = 0
                    else:
                        cover[l] = cover[2 * l] + cover[2 * l + 1]
                    l += 1
                if r & 1:
                    r -= 1
                    c = cnt[r] + val
                    cnt[r] = c
                    if c > 0:
                        cover[r] = span[r]
                    elif r >= sz:
                        cover[r] = 0
                    else:
                        cover[r] = cover[2 * r] + cover[2 * r + 1]
                l >>= 1
                r >>= 1
            # Tính lại cover cho toàn bộ tổ tiên của hai biên lên tới gốc.
            k = ll >> 1
            while k >= 1:
                if cnt[k] > 0:
                    cover[k] = span[k]
                else:
                    cover[k] = cover[2 * k] + cover[2 * k + 1]
                k >>= 1
            k = (rr - 1) >> 1
            while k >= 1:
                if cnt[k] > 0:
                    cover[k] = span[k]
                else:
                    cover[k] = cover[2 * k] + cover[2 * k + 1]
                k >>= 1
        prev_x = x

    sys.stdout.write(str(area) + "\n")


main()
