import sys


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    q = int(data[idx]); idx += 1

    # t: đích (0-based) của teleporter duy nhất tại mỗi hành tinh.
    t = [int(data[idx + i]) - 1 for i in range(n)]
    idx += n

    LOG = 30  # 2^30 > 1e9 >= k_max

    # Binary lifting: up[j][v] = đỉnh đến được sau 2^j bước từ v; up[0] = t.
    # up[j][v] == up[j-1][up[j-1][v]] == prev[prev[v]] cho mọi v, nên cả mức tính
    # bằng list(map(prev.__getitem__, prev)) -- nhanh hơn hẳn vòng lặp Python.
    up = [t]
    for _ in range(1, LOG):
        prev = up[-1]
        up.append(list(map(prev.__getitem__, prev)))

    # Đọc mọi truy vấn vào các mảng phẳng (điểm xuất phát 0-based).
    xs = [int(data[idx + 2 * i]) - 1 for i in range(q)]
    ks = [int(data[idx + 2 * i + 1]) for i in range(q)]

    # Xử lý gộp: tại mức j chỉ dịch những truy vấn có bit j của k bật, dùng up[j].
    # Batch tránh vòng while từng truy vấn và dồn phần nặng vào tầng C.
    cur = xs
    for j in range(LOG):
        upj = up[j]
        cur = [upj[c] if (kk >> j) & 1 else c for c, kk in zip(cur, ks)]

    # Cộng lại 1 để khớp định dạng 1-indexed của CSES, gom in một lần.
    sys.stdout.write("\n".join(map(str, (c + 1 for c in cur))) + ("\n" if cur else ""))


if __name__ == "__main__":
    main()
