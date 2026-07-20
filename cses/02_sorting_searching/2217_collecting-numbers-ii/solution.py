# Collecting Numbers II - https://cses.fi/problemset/task/2217
# Số vòng (rounds) = 1 + số cặp giá trị liên tiếp (v, v+1) mà pos[v] > pos[v+1].
# Mỗi lần swap chỉ ảnh hưởng tới một vài cặp kề giá trị, nên cập nhật count tăng dần.

import sys


def main():
    data = sys.stdin.buffer.read().split()

    n = int(data[0])
    m = int(data[1])

    arr = [0] * (n + 2)   # arr[vị trí] = giá trị  (các vị trí 1..n)
    pos = [0] * (n + 2)   # pos[giá trị] = vị trí

    for i in range(1, n + 1):
        v = int(data[1 + i])
        arr[i] = v
        pos[v] = i

    # Đếm số cặp "bad": pos[v] > pos[v+1] với v = 1..n-1
    count = 0
    for v in range(1, n):
        if pos[v] > pos[v + 1]:
            count += 1

    out = []
    j = 2 + n   # chỉ số bắt đầu của các truy vấn trong mảng data
    for _ in range(m):
        a = int(data[j]); b = int(data[j + 1]); j += 2

        if a != b:
            va = arr[a]
            vb = arr[b]

            # Bốn chỉ số cặp có thể bị ảnh hưởng (0 nghĩa là không hợp lệ,
            # vì chỉ số cặp hợp lệ nằm trong [1, n-1]).
            i1 = va - 1 if va - 1 >= 1 else 0
            i2 = va if va <= n - 1 else 0
            i3 = vb - 1 if vb - 1 >= 1 else 0
            i4 = vb if vb <= n - 1 else 0
            # Khử trùng lặp: chỉ có thể trùng khi va, vb là hai giá trị kề nhau.
            # vb = va + 1 -> i2 (cặp (va, va+1)) trùng i3 (cặp (vb-1, vb)).
            if i3 == i2:
                i3 = 0
            # vb = va - 1 -> i1 (cặp (va-1, va)) trùng i4 (cặp (vb, vb+1)).
            if i4 == i1:
                i4 = 0

            # Bỏ đóng góp cũ của các cặp bị ảnh hưởng
            if i1 and pos[i1] > pos[i1 + 1]: count -= 1
            if i2 and pos[i2] > pos[i2 + 1]: count -= 1
            if i3 and pos[i3] > pos[i3 + 1]: count -= 1
            if i4 and pos[i4] > pos[i4 + 1]: count -= 1

            # Thực hiện swap tại vị trí a, b (tức đổi vị trí của giá trị va, vb)
            arr[a] = vb
            arr[b] = va
            pos[va] = b
            pos[vb] = a

            # Thêm đóng góp mới của các cặp bị ảnh hưởng
            if i1 and pos[i1] > pos[i1 + 1]: count += 1
            if i2 and pos[i2] > pos[i2 + 1]: count += 1
            if i3 and pos[i3] > pos[i3 + 1]: count += 1
            if i4 and pos[i4] > pos[i4 + 1]: count += 1

        out.append(count + 1)

    sys.stdout.write('\n'.join(map(str, out)))
    sys.stdout.write('\n')


main()
