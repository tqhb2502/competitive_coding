import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    s = data[0]
    n = len(s)

    # Xâu 1-indexed với sentinel a[0] = -1 (khác mọi mã kí tự 0..25)
    # để bảo đảm chỉ số i - length[cur] - 1 luôn hợp lệ (>= 0).
    base = 97  # ord('a')
    a = [-1] * (n + 1)
    for i in range(1, n + 1):
        a[i] = s[i - 1] - base

    # Eertree: nút 0 = imaginary root (len = -1), nút 1 = empty root (len = 0).
    length = [-1, 0]
    link = [0, 0]
    trans = {}            # key = node * 26 + c  ->  nút con (palindrome c + P + c)
    last = 1              # palindrome-suffix dài nhất của tiền tố hiện tại
    ans = [0] * n

    # Đưa về biến cục bộ cho nhanh.
    _length = length
    _link = link
    _trans = trans

    # Thêm từng kí tự từ trái sang phải.
    for i in range(1, n + 1):
        c = a[i]
        # Tìm cur: suffix-palindrome dài nhất có thể mở rộng bằng kí tự c.
        cur = last
        while a[i - _length[cur] - 1] != c:
            cur = _link[cur]

        key = cur * 26 + c
        child = _trans.get(key)
        if child is not None:
            # Palindrome c + P(cur) + c đã tồn tại, chỉ cập nhật last.
            last = child
            ans[i - 1] = _length[child]
            continue

        # Tạo nút palindrome mới với độ dài length[cur] + 2.
        newlen = _length[cur] + 2
        if newlen == 1:
            sl = 1  # link tới empty root
        else:
            # Tìm suffix link cho nút mới bằng cách đi tiếp theo suffix link từ link[cur].
            tmp = _link[cur]
            while a[i - _length[tmp] - 1] != c:
                tmp = _link[tmp]
            sl = _trans[tmp * 26 + c]

        node = len(_length)
        _length.append(newlen)
        _link.append(sl)
        _trans[key] = node
        last = node
        # length[last] = độ dài palindrome-suffix dài nhất kết thúc tại i.
        ans[i - 1] = newlen

    sys.stdout.write(' '.join(map(str, ans)))
    sys.stdout.write('\n')


main()
