# Point Location Test - https://cses.fi/problemset/task/2189
# For each query decide if p3 is LEFT / RIGHT / TOUCH relative to the
# directed line p1 -> p2, using the integer cross product (exact arithmetic).
import sys


def main():
    data = sys.stdin.buffer.read().split()
    t = int(data[0])
    idx = 1
    out = []
    for _ in range(t):
        x1 = int(data[idx]); y1 = int(data[idx + 1])
        x2 = int(data[idx + 2]); y2 = int(data[idx + 3])
        x3 = int(data[idx + 4]); y3 = int(data[idx + 5])
        idx += 6
        # cross = (p2 - p1) x (p3 - p1)
        cross = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1)
        if cross > 0:
            out.append("LEFT")
        elif cross < 0:
            out.append("RIGHT")
        else:
            out.append("TOUCH")
    sys.stdout.write("\n".join(out) + "\n")


main()
