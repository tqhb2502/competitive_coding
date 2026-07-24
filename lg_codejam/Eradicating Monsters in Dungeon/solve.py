import sys


LIMIT = 100_000_000
INF = 10 ** 40


def min_quadratic_on_integer_interval(L, R, a, b):
    """
    Minimize f(s) = s^2 + a*s + b
    for integer s in [L, R].
    """
    if L > R:
        return INF

    # Real vertex is at s = -a / 2.
    c = (-a) // 2

    best = INF

    # Check bounds and several integer points around the vertex.
    for s in (L, R, c - 2, c - 1, c, c + 1, c + 2):
        if s < L:
            s = L
        elif s > R:
            s = R

        value = s * s + a * s + b
        if value < best:
            best = value

    return best


def solve_case(monsters):
    monsters.sort()

    n = len(monsters)
    p = [x for x, _ in monsters]
    w = [x for _, x in monsters]

    prefix = [0] * (n + 1)
    for i in range(n):
        prefix[i + 1] = prefix[i] + w[i]

    total_w = prefix[n]

    # Flattened DP arrays:
    # index(l, r) = l * n + r
    dpL = [INF] * (n * n)
    dpR = [INF] * (n * n)

    # Initialization: choose the first monster to eradicate.
    for k in range(n):
        pk = p[k]

        # Start from the left side of pk, then move right to pk.
        # Valid integer s: previous monster < s <= pk
        L = -LIMIT if k == 0 else p[k - 1] + 1
        R = pk

        # Cost: s^2 + total_w * (pk - s)
        #      = s^2 - total_w*s + total_w*pk
        best = min_quadratic_on_integer_interval(
            L,
            R,
            -total_w,
            total_w * pk
        )

        # Start from the right side of pk, then move left to pk.
        # Valid integer s: pk <= s < next monster
        L = pk
        R = LIMIT if k == n - 1 else p[k + 1] - 1

        # Cost: s^2 + total_w * (s - pk)
        #      = s^2 + total_w*s - total_w*pk
        candidate = min_quadratic_on_integer_interval(
            L,
            R,
            total_w,
            -total_w * pk
        )

        if candidate < best:
            best = candidate

        idx = k * n + k
        dpL[idx] = best
        dpR[idx] = best

    # DP by interval length.
    for length in range(1, n):
        for l in range(0, n - length + 1):
            r = l + length - 1
            idx = l * n + r

            curL = dpL[idx]
            curR = dpR[idx]

            killed_w = prefix[r + 1] - prefix[l]
            rem = total_w - killed_w

            # Expand to the left: kill monster l - 1.
            if l > 0:
                new_idx = (l - 1) * n + r

                # From p[l] to p[l - 1].
                cand = curL + rem * (p[l] - p[l - 1])
                if cand < dpL[new_idx]:
                    dpL[new_idx] = cand

                # From p[r] to p[l - 1].
                cand = curR + rem * (p[r] - p[l - 1])
                if cand < dpL[new_idx]:
                    dpL[new_idx] = cand

            # Expand to the right: kill monster r + 1.
            if r + 1 < n:
                new_idx = l * n + (r + 1)

                # From p[l] to p[r + 1].
                cand = curL + rem * (p[r + 1] - p[l])
                if cand < dpR[new_idx]:
                    dpR[new_idx] = cand

                # From p[r] to p[r + 1].
                cand = curR + rem * (p[r + 1] - p[r])
                if cand < dpR[new_idx]:
                    dpR[new_idx] = cand

    final_idx = n - 1  # index(0, n - 1)
    return min(dpL[final_idx], dpR[final_idx])


def solve():
    input_data = sys.stdin.buffer.read().split()
    t = int(input_data[0])
    ptr = 1

    answers = []

    for _ in range(t):
        n = int(input_data[ptr])
        ptr += 1

        monsters = []
        for _ in range(n):
            p_i = int(input_data[ptr])
            w_i = int(input_data[ptr + 1])
            ptr += 2
            monsters.append((p_i, w_i))

        answers.append(str(solve_case(monsters)))

    print("\n".join(answers))


if __name__ == "__main__":
    solve()
