# Tower of Hanoi - https://cses.fi/problemset/task/2165
import sys


def main():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])

    moves = []

    # Iterative recursion via explicit stack to avoid recursion limits,
    # though n <= 16 so plain recursion would also be fine.
    # frame: (m, src, dst, aux, stage)
    stack = [(n, 1, 3, 2, 0)]
    while stack:
        m, src, dst, aux, stage = stack.pop()
        if m == 0:
            continue
        if stage == 0:
            # push the continuation, then the first subproblem
            stack.append((m, src, dst, aux, 1))
            stack.append((m - 1, src, aux, dst, 0))
        elif stage == 1:
            moves.append((src, dst))
            stack.append((m - 1, aux, dst, src, 0))

    out = [str(len(moves))]
    for a, b in moves:
        out.append(str(a) + " " + str(b))
    sys.stdout.write("\n".join(out) + "\n")


main()
