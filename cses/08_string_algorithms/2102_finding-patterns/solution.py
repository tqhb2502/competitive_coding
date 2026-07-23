import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    s = data[0]
    k = int(data[1]) if len(data) > 1 else 0
    patterns = data[2:2 + k]

    n = len(s)
    max_states = 2 * n + 5

    # Suffix Automaton lưu song song bằng 3 mảng để tránh chi phí object.
    sa_len = [0] * max_states      # độ dài chuỗi dài nhất của mỗi trạng thái
    sa_link = [-1] * max_states    # suffix link
    sa_next = [None] * max_states  # dict transition cho mỗi trạng thái

    # state 0 là trạng thái khởi đầu.
    sa_next[0] = {}
    sa_len[0] = 0
    sa_link[0] = -1
    size = 1
    last = 0

    # Xây SAM online: thêm lần lượt từng ký tự c (giá trị byte) của text.
    for c in s:
        # Tạo trạng thái mới cur với len tăng 1.
        cur = size
        sa_len[cur] = sa_len[last] + 1
        sa_link[cur] = -1
        sa_next[cur] = {}
        size += 1

        # Đi ngược suffix link, nối cạnh về cur ở những nơi còn thiếu.
        p = last
        while p != -1 and c not in sa_next[p]:
            sa_next[p][c] = cur
            p = sa_link[p]

        if p == -1:
            sa_link[cur] = 0  # đi hết chuỗi suffix link -> link về state 0
        else:
            q = sa_next[p][c]
            if sa_len[p] + 1 == sa_len[q]:
                sa_link[cur] = q  # q liền kề -> nối trực tiếp
            else:
                # Clone q để tách lớp endpos rồi chuyển hướng các cạnh về clone.
                clone = size
                sa_len[clone] = sa_len[p] + 1
                sa_next[clone] = dict(sa_next[q])
                sa_link[clone] = sa_link[q]
                size += 1
                while p != -1 and sa_next[p].get(c) == q:
                    sa_next[p][c] = clone
                    p = sa_link[p]
                sa_link[q] = clone
                sa_link[cur] = clone
        last = cur

    # Với mỗi pattern, chạy trên automaton từ state 0: đọc hết được -> "YES".
    trans = sa_next
    out = []
    ap = out.append
    for pat in patterns:
        state = 0
        ok = True
        for c in pat:
            nxt = trans[state]
            t = nxt.get(c, -1)
            if t == -1:
                ok = False
                break
            state = t
        ap(b"YES" if ok else b"NO")

    sys.stdout.buffer.write(b"\n".join(out))
    if out:
        sys.stdout.buffer.write(b"\n")


main()
