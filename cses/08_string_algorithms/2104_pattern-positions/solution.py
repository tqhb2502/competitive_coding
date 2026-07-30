import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    s = data[0]                     # xâu s dạng bytes; lặp qua trả về các số int
    # data[1] là k; các pattern nằm ở phần còn lại.
    patterns = data[2:]

    n = len(s)
    max_states = 2 * n + 5

    # Các mảng song song mô tả từng trạng thái của Suffix Automaton (SAM).
    trans = [None] * max_states     # danh sách dict: byte -> trạng thái
    link = [0] * max_states         # suffix link
    length = [0] * max_states       # độ dài xâu dài nhất của lớp tương đương
    firstpos = [0] * max_states     # vị trí kết thúc (0-indexed) sớm nhất

    # Trạng thái khởi đầu.
    trans[0] = {}
    link[0] = -1
    length[0] = 0
    sz = 1
    last = 0

    # Xây SAM online: thêm lần lượt từng ký tự của s.
    for pos in range(n):
        c = s[pos]
        # Tạo trạng thái mới cur cho tiền tố hiện tại; firstpos[cur] = pos là vị
        # trí kết thúc lần xuất hiện đầu tiên.
        cur = sz
        sz += 1
        trans[cur] = {}
        length[cur] = length[last] + 1
        firstpos[cur] = length[cur] - 1          # == pos
        # Nối transition dọc theo chuỗi suffix link của last.
        p = last
        while p != -1 and c not in trans[p]:
            trans[p][c] = cur
            p = link[p]
        if p == -1:
            link[cur] = 0
        else:
            q = trans[p][c]
            if length[p] + 1 == length[q]:
                link[cur] = q
            else:
                # Tách (clone) q; clone kế thừa firstpos[q] vì endpos chỉ mở rộng
                # thêm vị trí mới (lớn hơn) nên giá trị nhỏ nhất không đổi.
                clone = sz
                sz += 1
                length[clone] = length[p] + 1
                trans[clone] = dict(trans[q])
                link[clone] = link[q]
                firstpos[clone] = firstpos[q]
                while p != -1 and trans[p].get(c) == q:
                    trans[p][c] = clone
                    p = link[p]
                link[q] = clone
                link[cur] = clone
        last = cur

    # Trả lời truy vấn. Nội địa hoá biến để tăng tốc.
    res = []
    append = res.append
    tr = trans
    fp = firstpos
    for pat in patterns:
        # Đi từng ký tự của pattern trong SAM.
        v = 0
        ok = True
        for c in pat:
            nxt = tr[v].get(c)
            if nxt is None:         # thiếu transition -> không phải xâu con
                ok = False
                break
            v = nxt
        if ok:
            # Đổi vị trí kết thúc sớm nhất sang vị trí bắt đầu (1-indexed).
            append(fp[v] - len(pat) + 2)
        else:
            append(-1)

    sys.stdout.write('\n'.join(map(str, res)))
    sys.stdout.write('\n')


main()
