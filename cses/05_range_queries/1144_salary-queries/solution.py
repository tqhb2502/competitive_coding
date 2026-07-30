import sys
from bisect import bisect_left, bisect_right


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    q = int(data[idx]); idx += 1

    p = [0] * n
    for i in range(n):
        p[i] = int(data[idx]); idx += 1

    # Đọc offline toàn bộ truy vấn, gom mọi lương có thể xuất hiện để nén tọa độ:
    # gồm lương ban đầu p_i và mọi lương mới x của các truy vấn cập nhật "!".
    qtype = [0] * q     # 1 = update "!", 0 = query "?"
    qa = [0] * q
    qb = [0] * q
    all_vals = list(p)
    for j in range(q):
        t = data[idx]; idx += 1
        aa = int(data[idx]); idx += 1
        bb = int(data[idx]); idx += 1
        qa[j] = aa
        qb[j] = bb
        if t == b'!':
            qtype[j] = 1        # aa = k, bb = x (lương mới)
            all_vals.append(bb)
        else:
            qtype[j] = 0        # aa = a, bb = b (khoảng truy vấn)

    # Coordinate compression: tập giá trị duy nhất đã sắp xếp.
    vals = sorted(set(all_vals))
    m = len(vals)

    # Fenwick tree (BIT) đếm tần suất lương theo vị trí nén.
    tree = [0] * (m + 1)

    # cur[k] = vị trí nén (1-indexed) của lương hiện tại của nhân viên k.
    cur = [0] * (n + 1)

    # Khởi tạo BIT: cộng 1 vào vị trí nén của lương ban đầu từng nhân viên.
    for i in range(n):
        pos = bisect_left(vals, p[i]) + 1
        cur[i + 1] = pos
        # update(pos, +1): lan lên các nút cha theo j & -j.
        j = pos
        while j <= m:
            tree[j] += 1
            j += j & (-j)

    out = []
    ap = out.append
    for jq in range(q):
        a = qa[jq]
        b = qb[jq]
        if qtype[jq] == 1:
            # "! k x": rút 1 khỏi vị trí lương cũ rồi cộng 1 vào vị trí lương mới.
            k = a
            x = b
            # update(cur[k], -1)
            j = cur[k]
            while j <= m:
                tree[j] -= 1
                j += j & (-j)
            pos = bisect_left(vals, x) + 1
            cur[k] = pos
            # update(pos, +1)
            j = pos
            while j <= m:
                tree[j] += 1
                j += j & (-j)
        else:
            # "? a b": đáp án = prefix(hi) - prefix(lo).
            hi = bisect_right(vals, b)   # số lương <= b
            lo = bisect_left(vals, a)    # số lương < a
            s = 0
            j = hi
            while j > 0:
                s += tree[j]
                j -= j & (-j)
            j = lo
            while j > 0:
                s -= tree[j]
                j -= j & (-j)
            ap(s)

    sys.stdout.buffer.write(('\n'.join(map(str, out)) + ('\n' if out else '')).encode())


main()
