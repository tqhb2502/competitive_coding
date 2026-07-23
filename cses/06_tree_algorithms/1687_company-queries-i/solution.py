import sys


def main():
    # Đọc toàn bộ input một lần để tăng tốc
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    q = int(data[idx]); idx += 1

    LOG = 18  # 2^18 = 262144 > 2*10^5 >= n

    # parent[v] = cha trực tiếp của v (1-indexed); parent[1] = 0 vì gốc không có cha
    parent = [0] * (n + 1)
    for v in range(2, n + 1):
        parent[v] = int(data[idx]); idx += 1

    # Binary lifting lưu dạng mảng phẳng: up[j*size + v] = tổ tiên cách v đúng 2^j bậc
    # (node ảo 0 hấp thụ mọi bước nhảy vượt gốc)
    size = n + 1
    up = [0] * (LOG * size)

    # Tầng 0 chính là cha trực tiếp
    for v in range(1, n + 1):
        up[v] = parent[v]

    # Tầng cao dựng theo công thức 2^j = 2^(j-1) + 2^(j-1)
    for j in range(1, LOG):
        base = j * size
        prev = (j - 1) * size
        for v in range(1, n + 1):
            up[base + v] = up[prev + up[prev + v]]

    out = []
    for _ in range(q):
        x = int(data[idx]); k = int(data[idx + 1]); idx += 2
        # Nhảy theo từng bit được bật của k; dừng sớm nếu x đã rơi về node ảo 0
        j = 0
        kk = k
        while kk and x:
            if kk & 1:
                x = up[j * size + x]
            kk >>= 1
            j += 1
        # x == 0 nghĩa là tổ tiên không tồn tại -> in -1
        out.append(str(x) if x else "-1")

    sys.stdout.buffer.write("\n".join(out).encode())
    sys.stdout.buffer.write(b"\n")


if __name__ == "__main__":
    main()
