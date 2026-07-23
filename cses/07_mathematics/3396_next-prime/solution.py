import sys

# Tập base cho Miller-Rabin deterministic: đúng với mọi n < 3.3 * 10^24.
_MR_BASES = (2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37)


def is_prime(n):
    if n < 2:
        return False
    # Chia thử nhanh với các số nguyên tố nhỏ.
    for p in (2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37):
        if n % p == 0:
            return n == p
    # Viết n - 1 = d * 2^s với d lẻ.
    d = n - 1
    s = 0
    while d % 2 == 0:
        d //= 2
        s += 1
    # Kiểm tra Miller-Rabin với từng base.
    for a in _MR_BASES:
        x = pow(a, d, n)
        if x == 1 or x == n - 1:
            continue
        # Bình phương liên tiếp tối đa s-1 lần, tìm giá trị n-1.
        for _ in range(s - 1):
            x = x * x % n
            if x == n - 1:
                break
        else:
            # Không gặp n-1 => n là hợp số.
            return False
    return True


def next_prime(n):
    # Số nguyên tố nhỏ nhất lớn hơn n.
    if n < 2:
        return 2
    cand = n + 1
    if cand == 2:
        return 2
    # Đưa về candidate lẻ đầu tiên (> 2), sau đó chỉ duyệt số lẻ.
    if cand % 2 == 0:
        cand += 1
    while not is_prime(cand):
        cand += 2
    return cand


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    t = int(data[0])
    out = []
    for i in range(1, t + 1):
        n = int(data[i])
        out.append(str(next_prime(n)))
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
