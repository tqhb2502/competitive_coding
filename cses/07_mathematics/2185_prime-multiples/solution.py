import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    k = int(data[1])
    primes = [int(x) for x in data[2:2 + k]]

    answer = 0
    # dp lưu (tích_tập_con, số_phần_tử); bắt đầu bằng tập rỗng (tích = 1, cnt = 0).
    dp = [(1, 0)]
    for p in primes:
        additions = []
        # Mở rộng mọi tập con hiện có bằng cách thêm số nguyên tố p.
        for prod, cnt in dp:
            np = prod * p
            # Cắt tỉa: chỉ giữ tập con có tích <= n (nếu không thì đóng góp bằng 0).
            if np <= n:
                new_cnt = cnt + 1
                additions.append((np, new_cnt))
                # Bao hàm - loại trừ: tập con lẻ cộng, tập con chẵn trừ.
                if new_cnt & 1:
                    answer += n // np
                else:
                    answer -= n // np
        dp.extend(additions)

    sys.stdout.write(str(answer) + "\n")


main()
