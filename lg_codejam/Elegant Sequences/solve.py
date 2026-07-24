import sys


def solve():
    data = list(map(int, sys.stdin.buffer.read().split()))
    N, K = data[0], data[1]
    A = data[2:]

    S = [0] * (N + 1)
    Q = [0] * (N + 1)

    for i, x in enumerate(A, 1):
        S[i] = S[i - 1] + x
        Q[i] = Q[i - 1] + x * x

    def cross(i, j, k):
        """
        Cross product of vector P_i -> P_j and P_i -> P_k.
        P_t = (S[t], Q[t])
        """
        return (S[j] - S[i]) * (Q[k] - Q[i]) - (Q[j] - Q[i]) * (S[k] - S[i])

    hull = []

    best_num = -1
    best_den = 1

    for r in range(K, N + 1):
        new_idx = r - K

        # Add P_{r-K} into lower convex hull.
        while len(hull) >= 2 and cross(hull[-2], hull[-1], new_idx) <= 0:
            hull.pop()
        hull.append(new_idx)

        x_r = S[r]
        y_r = Q[r]

        # Binary search on hull to find point giving maximum slope to P_r.
        lo, hi = 0, len(hull) - 1

        while lo < hi:
            mid = (lo + hi) // 2

            u = hull[mid]
            v = hull[mid + 1]

            # Compare:
            # slope(P_v, P_r) >= slope(P_u, P_r)
            #
            # (Q[r] - Q[v]) / (S[r] - S[v])
            # >=
            # (Q[r] - Q[u]) / (S[r] - S[u])
            #
            # Denominators are positive, so cross-multiplication is safe.
            if (y_r - Q[v]) * (x_r - S[u]) >= (y_r - Q[u]) * (x_r - S[v]):
                lo = mid + 1
            else:
                hi = mid

        left = hull[lo]

        num = Q[r] - Q[left]
        den = S[r] - S[left]

        if num * best_den > best_num * den:
            best_num = num
            best_den = den

    print(f"{best_num / best_den:.12f}")


if __name__ == "__main__":
    solve()
