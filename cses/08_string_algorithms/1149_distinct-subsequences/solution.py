import sys


def main() -> None:
    data = sys.stdin.buffer.read().split()
    if not data:
        sys.stdout.write("0\n")
        return
    s = data[0]

    MOD = 1_000_000_007
    dp = 1               # dp: số subsequence phân biệt tính cả chuỗi rỗng
    last = [0] * 26      # last[c] = dp[j-1] của lần xuất hiện gần nhất của c
    seen = [False] * 26

    for byte in s:
        c = byte - 97    # 'a' == 97
        # Gấp đôi: mỗi chuỗi cũ có thể giữ nguyên hoặc gắn thêm ký tự hiện tại.
        new = (2 * dp) % MOD
        # Trừ phần bị đếm trùng do ký tự này đã từng xuất hiện trước đó.
        if seen[c]:
            new = (new - last[c]) % MOD
        last[c] = dp
        seen[c] = True
        dp = new

    # Trừ 1 để loại chuỗi rỗng.
    ans = (dp - 1) % MOD
    sys.stdout.write(str(ans) + "\n")


main()
