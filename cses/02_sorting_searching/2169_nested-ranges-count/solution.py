# Nested Ranges Count - https://cses.fi/problemset/task/2169
# Với mỗi range [a, b]: đếm số range nó CHỨA và số range CHỨA nó.
# Dùng nén tọa độ trên b + Fenwick tree (BIT), quét hai lần.
import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    ints = list(map(int, data[1:1 + 2 * n]))
    a = ints[0::2]
    b = ints[1::2]

    # Nén tọa độ cho b (rank 1-indexed).
    sorted_b = sorted(set(b))
    rank = {v: idx + 1 for idx, v in enumerate(sorted_b)}
    m = len(sorted_b)
    br = [rank[v] for v in b]

    contains = [0] * n   # số range mà i chứa
    contained = [0] * n  # số range chứa i

    # Lưu ý: các range đôi một khác nhau => quan hệ "chứa" là phản đối xứng,
    # không có trường hợp hai range chứa lẫn nhau.
    # BIG lớn hơn mọi giá trị b (b <= 10^9 < 2^31) nên có thể ghép hai khóa
    # sắp xếp thành một số nguyên duy nhất, giúp sort nhanh hơn so với tuple.
    BIG = 1 << 31

    # ------------------------------------------------------------------
    # Pass 1: contains[i] = số j thỏa a_i <= a_j và b_j <= b_i.
    # Sắp xếp theo a giảm dần, hòa thì b tăng dần => khóa = -a * BIG + b.
    # Khi xử lý i, mọi j đã chèn đều có a_j >= a_i; đếm b_j <= b_i.
    # ------------------------------------------------------------------
    key = [b[i] - a[i] * BIG for i in range(n)]
    order = sorted(range(n), key=key.__getitem__)
    tree = [0] * (m + 1)
    for i in order:
        x = br[i]
        s = 0
        while x > 0:
            s += tree[x]
            x &= x - 1            # xóa bit thấp nhất (đi lên tổ tiên prefix)
        contains[i] = s
        x = br[i]
        while x <= m:
            tree[x] += 1
            x += x & (-x)

    # ------------------------------------------------------------------
    # Pass 2: contained[i] = số j thỏa a_j <= a_i và b_i <= b_j.
    # Sắp xếp theo a tăng dần, hòa thì b giảm dần => khóa = a * BIG - b.
    # Khi xử lý i, mọi j đã chèn đều có a_j <= a_i; cần đếm b_j >= b_i:
    # contained[i] = (số đã chèn) - (số b_j < b_i).
    # ------------------------------------------------------------------
    key = [a[i] * BIG - b[i] for i in range(n)]
    order = sorted(range(n), key=key.__getitem__)
    tree = [0] * (m + 1)
    cnt = 0
    for i in order:
        x = br[i] - 1
        s = 0
        while x > 0:
            s += tree[x]
            x &= x - 1
        contained[i] = cnt - s
        cnt += 1
        x = br[i]
        while x <= m:
            tree[x] += 1
            x += x & (-x)

    sys.stdout.write(' '.join(map(str, contains)) + '\n' +
                     ' '.join(map(str, contained)) + '\n')


main()
