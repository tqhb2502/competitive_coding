import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        sys.stdout.write("0\n")
        return
    s = data[0]  # bytes; mỗi phần tử là một int (giá trị byte của ký tự)

    # Suffix automaton (SAM): trạng thái 0 là trạng thái gốc (initial).
    trans = [dict()]   # transitions của từng trạng thái
    link = [-1]        # suffix link
    length = [0]       # độ dài xâu con dài nhất của lớp tương đương
    last = 0

    # Thêm lần lượt từng ký tự vào cuối, mở rộng SAM online.
    for c in s:
        # Tạo trạng thái mới cho tiền tố hiện tại.
        cur = len(length)
        length.append(length[last] + 1)
        link.append(-1)
        trans.append({})

        # Đi theo chuỗi suffix link, gắn cạnh tới trạng thái mới khi còn thiếu.
        p = last
        while p != -1 and c not in trans[p]:
            trans[p][c] = cur
            p = link[p]

        if p == -1:
            # Không gặp cạnh nào: suffix link trỏ về gốc.
            link[cur] = 0
        else:
            q = trans[p][c]
            if length[p] + 1 == length[q]:
                # q liền kề, dùng trực tiếp làm suffix link.
                link[cur] = q
            else:
                # Tách lớp: tạo trạng thái clone sao chép q rồi chỉnh len.
                clone = len(length)
                length.append(length[p] + 1)
                link.append(link[q])
                trans.append(dict(trans[q]))
                # Chuyển hướng các cạnh đang trỏ tới q sang clone.
                while p != -1 and trans[p].get(c) == q:
                    trans[p][c] = clone
                    p = link[p]
                link[q] = clone
                link[cur] = clone
        last = cur

    # Mỗi trạng thái v != gốc đóng góp len[v] - len[link[v]] xâu con mới.
    ans = 0
    for v in range(1, len(length)):
        ans += length[v] - length[link[v]]
    sys.stdout.write(str(ans) + "\n")


main()
