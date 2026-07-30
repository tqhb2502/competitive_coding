import sys


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0
    n = int(data[pos]); pos += 1
    q = int(data[pos]); pos += 1

    # Đọc mảng ban đầu (đánh chỉ số từ 1).
    x = [0] * (n + 1)
    for i in range(1, n + 1):
        x[i] = int(data[pos]); pos += 1

    # Đọc trước toàn bộ truy vấn để nén tọa độ cho cả các update sau này.
    queries = []
    for _ in range(q):
        t = int(data[pos]); pos += 1
        if t == 1:
            k = int(data[pos]); u = int(data[pos + 1]); pos += 2
            queries.append((1, k, u))
        else:
            a = int(data[pos]); b = int(data[pos + 1]); pos += 2
            queries.append((2, a, b))

    # --- Nén giá trị (mảng ban đầu + mọi giá trị đích u của các update) ---
    vals = set()
    for i in range(1, n + 1):
        vals.add(x[i])
    for t, a, b in queries:
        if t == 1:
            vals.add(b)          # b chính là u trong truy vấn update
    value_rank = {v: r for r, v in enumerate(sorted(vals))}

    # --- Nén MỌI cặp (value, position) có thể active, xếp theo (value_rank, position) ---
    # Trong thứ tự này, hàng xóm cùng giá trị của một cặp là hàng xóm toàn cục liền kề.
    N1 = n + 1                   # mã hoá khoá = value_rank * N1 + position
    cur_enc = [0] * (n + 1)
    encs = set()
    for i in range(1, n + 1):
        e = value_rank[x[i]] * N1 + i
        cur_enc[i] = e
        encs.add(e)
    for t, k, u in queries:
        if t == 1:
            encs.add(value_rank[u] * N1 + k)

    senc = sorted(encs)          # đã sắp theo (value_rank, position)
    M = len(senc)
    key2idx = {}
    k_pos = [0] * (M + 1)        # vị trí lưu tại một chỉ số nén
    k_vid = [0] * (M + 1)        # value_rank lưu tại một chỉ số nén
    for i, e in enumerate(senc):
        c = i + 1
        key2idx[e] = c
        k_pos[c] = e % N1
        k_vid[c] = e // N1

    cur_idx = [0] * (n + 1)      # chỉ số nén của cặp đang active tại mỗi vị trí
    for i in range(1, n + 1):
        cur_idx[i] = key2idx[cur_enc[i]]

    # --- Fenwick/BIT trên các chỉ số nén: lưu cờ present + hỗ trợ select ---
    bit = [0] * (M + 1)

    def bit_add(i, d, bit=bit, M=M):
        # Bật/tắt một cặp (activate / deactivate).
        while i <= M:
            bit[i] += d
            i += i & (-i)

    def bit_sum(i, bit=bit):
        # Đếm số cặp active trong tiền tố [1..i].
        s = 0
        while i > 0:
            s += bit[i]
            i -= i & (-i)
        return s

    LOGM = M.bit_length()

    def bit_select(rank, bit=bit, M=M, LOGM=LOGM):
        # Binary lifting: chỉ số của phần tử active thứ "rank" (1-based).
        p = 0
        r = rank
        j = LOGM
        while j >= 0:
            nxt = p + (1 << j)
            if nxt <= M and bit[nxt] < r:
                p = nxt
                r -= bit[nxt]
            j -= 1
        return p + 1

    for i in range(1, n + 1):
        bit_add(cur_idx[i], 1)

    def in_val_pred(ix, vid):
        # Vị trí của predecessor cùng giá trị của chỉ số nén ix, hoặc 0 nếu không có.
        r = bit_sum(ix - 1)
        if r == 0:
            return 0
        j = bit_select(r)
        return k_pos[j] if k_vid[j] == vid else 0

    def in_val_succ(ix, vid, tot):
        # Vị trí của successor cùng giá trị của chỉ số nén ix, hoặc 0 nếu không có.
        r = bit_sum(ix)
        if r >= tot:
            return 0
        j = bit_select(r + 1)
        return k_pos[j] if k_vid[j] == vid else 0

    # --- prev[] ban đầu: last[v] = vị trí gần nhất mang giá trị v ---
    prev_arr = [0] * (n + 1)
    last = {}
    for i in range(1, n + 1):
        v = x[i]
        prev_arr[i] = last.get(v, 0)
        last[v] = i

    # --- Segment tree lặp cho range-max của prev[] (prev có thể giảm nên không dùng BIT) ---
    size = 1
    while size < n:
        size <<= 1
    seg = [0] * (2 * size)
    for i in range(n):
        seg[size + i] = prev_arr[i + 1]
    for i in range(size - 1, 0, -1):
        a2 = seg[2 * i]; b2 = seg[2 * i + 1]
        seg[i] = a2 if a2 > b2 else b2

    def seg_update(p, val, seg=seg, size=size):
        # Gán điểm: đặt prev[p] = val rồi cập nhật dọc lên gốc.
        i = size + p - 1
        seg[i] = val
        i >>= 1
        while i:
            a2 = seg[2 * i]; b2 = seg[2 * i + 1]
            seg[i] = a2 if a2 > b2 else b2
            i >>= 1

    def seg_query(l, r, seg=seg, size=size):
        # Max của prev[] trên đoạn [l, r].
        res = 0
        l0 = size + l - 1
        r0 = size + r
        while l0 < r0:
            if l0 & 1:
                if seg[l0] > res:
                    res = seg[l0]
                l0 += 1
            if r0 & 1:
                r0 -= 1
                if seg[r0] > res:
                    res = seg[r0]
            l0 >>= 1
            r0 >>= 1
        return res

    out = []
    YES = b"YES"
    NO = b"NO"
    for t, a, b in queries:
        # Truy vấn "2 a b": đoạn phân biệt <=> max(prev trên [a, b]) < a.
        if t == 2:
            out.append(YES if seg_query(a, b) < a else NO)
        else:
            # Update "1 k u": xác định cặp cũ (ov) và cặp mới (nv) của vị trí k.
            k = a
            u = b
            ov = cur_idx[k]
            nv = key2idx[value_rank[u] * N1 + k]
            if nv == ov:
                continue         # giá trị mới trùng giá trị hiện tại: bỏ qua
            # Gỡ k khỏi nhóm giá trị cũ: successor cũ nay trỏ prev về predecessor cũ.
            vid_o = k_vid[ov]
            bit_add(ov, -1)
            p_o = in_val_pred(ov, vid_o)
            s_o = in_val_succ(ov, vid_o, n - 1)
            if s_o:
                seg_update(s_o, p_o)
            # Chèn k vào nhóm giá trị mới: prev[k] = predecessor mới; successor mới đổi prev sang k.
            bit_add(nv, 1)
            vid_n = k_vid[nv]
            p_n = in_val_pred(nv, vid_n)
            s_n = in_val_succ(nv, vid_n, n)
            seg_update(k, p_n)
            if s_n:
                seg_update(s_n, k)
            cur_idx[k] = nv

    sys.stdout.buffer.write(b"\n".join(out))
    if out:
        sys.stdout.buffer.write(b"\n")


main()
