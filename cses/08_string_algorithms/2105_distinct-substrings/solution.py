# Distinct Substrings - https://cses.fi/problemset/task/2105
# Đếm số xâu con (substring) phân biệt của một xâu.
# Sử dụng suffix automaton (SAM): mỗi trạng thái v (khác trạng thái gốc)
# biểu diễn một tập các xâu con kết thúc tại v, số lượng xâu con mới trên
# trạng thái đó là len[v] - len[link[v]]. Tổng trên tất cả trạng thái chính
# là số xâu con phân biệt.
import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        sys.stdout.write("0\n")
        return
    s = data[0]  # bytes; mỗi phần tử là một int (giá trị byte)

    # Suffix automaton
    # trạng thái 0 là trạng thái khởi tạo (initial)
    trans = [dict()]   # transitions của từng trạng thái
    link = [-1]        # suffix link
    length = [0]       # độ dài xâu dài nhất của lớp tương đương
    last = 0

    for c in s:
        cur = len(length)
        length.append(length[last] + 1)
        link.append(-1)
        trans.append({})
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
                clone = len(length)
                length.append(length[p] + 1)
                link.append(link[q])
                trans.append(dict(trans[q]))
                while p != -1 and trans[p].get(c) == q:
                    trans[p][c] = clone
                    p = link[p]
                link[q] = clone
                link[cur] = clone
        last = cur

    ans = 0
    for v in range(1, len(length)):
        ans += length[v] - length[link[v]]
    sys.stdout.write(str(ans) + "\n")


main()
