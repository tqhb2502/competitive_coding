import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    if n <= 0:
        sys.stdout.write("0\n")
        return
    h = [int(x) for x in data[1:1 + n]]

    # SL[i] / SR[i]: núi gần nhất cao hơn i THỰC SỰ ở bên trái / phải
    # (-1 hoặc n nếu không tồn tại). Tính bằng monotonic stack O(n).
    SL = [-1] * n
    SR = [n] * n
    stack = []
    # Quét từ trái sang phải tìm cha bên trái.
    for i in range(n):
        hi = h[i]
        while stack and h[stack[-1]] <= hi:
            stack.pop()
        if stack:
            SL[i] = stack[-1]
        stack.append(i)
    # Quét từ phải sang trái tìm cha bên phải.
    stack.clear()
    for i in range(n - 1, -1, -1):
        hi = h[i]
        while stack and h[stack[-1]] <= hi:
            stack.pop()
        if stack:
            SR[i] = stack[-1]
        stack.append(i)

    # childmax[i] = max{dp[j]} trên đoạn lượn tới được của i.
    # Xử lý các núi theo chiều cao TĂNG DẦN để mọi đích (thấp hơn thực sự) đã có dp.
    childmax = [0] * n
    order = sorted(range(n), key=h.__getitem__)

    ans = 0
    for i in order:
        # dp[i] = 1 + max dp của các núi lượn tới được.
        d = childmax[i] + 1
        if d > ans:
            ans = d
        # "Đẩy" dp[i] lên hai cha SL và SR bằng phép max.
        l = SL[i]
        if l >= 0 and d > childmax[l]:
            childmax[l] = d
        r = SR[i]
        if r < n and d > childmax[r]:
            childmax[r] = d

    sys.stdout.write(str(ans) + "\n")


main()
