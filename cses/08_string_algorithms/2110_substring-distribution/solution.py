import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    s = data[0]                      # bytes; lặp qua cho ra các int
    n = len(s)

    # Các mảng của suffix automaton (SAM): len, suffix link, cạnh chuyển (dict)
    max_states = 2 * n + 5
    sa_len = [0] * max_states
    sa_link = [-1] * max_states
    sa_next = [None] * max_states
    sa_next[0] = {}                  # state khởi tạo 0
    size = 1
    last = 0

    # Xây dựng SAM online: thêm lần lượt từng ký tự của s
    for ch in s:                     # ch là int (giá trị byte)
        cur = size
        size += 1
        sa_len[cur] = sa_len[last] + 1
        sa_next[cur] = {}
        # Đi theo suffix link, thêm cạnh chuyển tới state mới
        p = last
        while p != -1 and ch not in sa_next[p]:
            sa_next[p][ch] = cur
            p = sa_link[p]
        if p == -1:
            # Không có cạnh trùng: suffix link trỏ về state khởi tạo
            sa_link[cur] = 0
        else:
            q = sa_next[p][ch]
            if sa_len[p] + 1 == sa_len[q]:
                # Cạnh liên tục: dùng thẳng q làm suffix link
                sa_link[cur] = q
            else:
                # Tách state (clone) để giữ tính tối thiểu của automaton
                clone = size
                size += 1
                sa_len[clone] = sa_len[p] + 1
                sa_next[clone] = dict(sa_next[q])
                sa_link[clone] = sa_link[q]
                # Chuyển các cạnh trỏ tới q sang clone
                while p != -1 and sa_next[p].get(ch) == q:
                    sa_next[p][ch] = clone
                    p = sa_link[p]
                sa_link[q] = clone
                sa_link[cur] = clone
        last = cur

    # Mảng hiệu: mỗi state cộng 1 vào các độ dài [len[link]+1, len]
    diff = [0] * (n + 2)
    for v in range(1, size):
        L = sa_len[sa_link[v]] + 1
        R = sa_len[v]
        diff[L] += 1
        diff[R + 1] -= 1

    # Prefix sum: giá trị tại L là số xâu con phân biệt có độ dài L
    out = []
    run = 0
    for L in range(1, n + 1):
        run += diff[L]
        out.append(run)

    sys.stdout.write(' '.join(map(str, out)))
    sys.stdout.write('\n')


main()
