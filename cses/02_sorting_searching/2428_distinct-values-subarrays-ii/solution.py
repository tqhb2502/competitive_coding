import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    k = int(data[1])
    # Giữ nguyên dạng token bytes: vừa hashable vừa so sánh được, tránh chi phí parse int.
    a = data[2:2 + n]

    # Nén tọa độ về [0, n) để dùng mảng đếm tần suất bằng list (nhanh hơn dict).
    comp = {}
    idx = 0
    arr = [0] * n
    for i in range(n):
        v = a[i]
        j = comp.get(v)
        if j is None:
            j = idx
            comp[v] = j
            idx += 1
        arr[i] = j

    # Two-pointer / sliding window: duy trì cửa sổ [left, right] có <= k giá trị phân biệt.
    cnt = [0] * idx
    distinct = 0
    left = 0
    total = 0
    for right in range(n):
        # Thêm phần tử bên phải; nếu tần suất cũ bằng 0 thì có thêm một giá trị mới.
        v = arr[right]
        if cnt[v] == 0:
            distinct += 1
        cnt[v] += 1
        # Thu nhỏ cửa sổ từ bên trái cho tới khi số giá trị phân biệt <= k.
        while distinct > k:
            lv = arr[left]
            cnt[lv] -= 1
            if cnt[lv] == 0:
                distinct -= 1
            left += 1
        # Mọi subarray kết thúc tại right, bắt đầu trong [left, right] đều hợp lệ.
        total += right - left + 1

    sys.stdout.write(str(total) + "\n")


main()
