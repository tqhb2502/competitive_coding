# All Palindromes - https://cses.fi/problemset/task/3138
# Cho mot xau, voi moi vi tri in ra do dai palindrome dai nhat KET THUC tai vi tri do.
# Dung Eertree (palindromic tree): sau khi them ki tu thu i, nut `last` chinh la
# palindrome-suffix dai nhat ket thuc tai i, nen length[last] la dap an cho vi tri i.
import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    s = data[0]
    n = len(s)

    # Xau 1-indexed voi sentinel a[0] = -1 (khac moi ki tu 0..25)
    # de bao dam chi so i - length[cur] - 1 luon hop le (>= 0).
    base = 97  # ord('a')
    a = [-1] * (n + 1)
    for i in range(1, n + 1):
        a[i] = s[i - 1] - base

    # Eertree:
    #   node 0 = imaginary root (len = -1), node 1 = empty root (len = 0)
    length = [-1, 0]
    link = [0, 0]
    trans = {}            # key = node * 26 + c  ->  node con (palindrome c + P + c)
    last = 1              # palindrome-suffix dai nhat cua tien to hien tai
    ans = [0] * n

    # tro thanh bien cuc bo cho nhanh
    _length = length
    _link = link
    _trans = trans

    for i in range(1, n + 1):
        c = a[i]
        # tim cur = suffix-palindrome dai nhat co the mo rong bang ki tu c
        cur = last
        while a[i - _length[cur] - 1] != c:
            cur = _link[cur]

        key = cur * 26 + c
        child = _trans.get(key)
        if child is not None:
            last = child
            ans[i - 1] = _length[child]
            continue

        # tao nut palindrome moi
        newlen = _length[cur] + 2
        if newlen == 1:
            sl = 1  # link toi empty root
        else:
            tmp = _link[cur]
            while a[i - _length[tmp] - 1] != c:
                tmp = _link[tmp]
            sl = _trans[tmp * 26 + c]

        node = len(_length)
        _length.append(newlen)
        _link.append(sl)
        _trans[key] = node
        last = node
        ans[i - 1] = newlen

    sys.stdout.write(' '.join(map(str, ans)))
    sys.stdout.write('\n')


main()
