import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    x = int(data[1])
    a = [int(v) for v in data[2:2 + n]]

    # hashmap: tổng cặp -> (k, l) với k < l, cả hai chỉ số đều đã cố định
    seen = {}
    get = seen.get

    for i in range(n):
        ai = a[i]
        # TÌM: coi i là chỉ số nhỏ của cặp cao; với mọi j > i tìm need trong seen
        for j in range(i + 1, n):
            p = get(x - ai - a[j])
            if p is not None:
                k, l = p
                sys.stdout.write("%d %d %d %d\n" % (k + 1, l + 1, i + 1, j + 1))
                return
        # THÊM: mọi cặp (k, i) với k < i (chỉ giữ cặp đầu tiên cho mỗi tổng),
        # thực hiện SAU bước tìm nên mọi cặp trong seen đều có chỉ số < i
        for k in range(i):
            s = a[k] + ai
            if s not in seen:
                seen[s] = (k, i)

    sys.stdout.write("IMPOSSIBLE\n")


main()
