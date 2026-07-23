import sys


def solve() -> None:
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1

    # Toạ độ các đỉnh đa giác theo thứ tự.
    xs = [0] * n
    ys = [0] * n
    for i in range(n):
        xs[i] = int(data[idx]); idx += 1
        ys[i] = int(data[idx]); idx += 1

    out = []
    for _ in range(m):
        px = int(data[idx]); idx += 1
        py = int(data[idx]); idx += 1

        inside = False
        on_boundary = False

        # Duyệt từng cạnh (x1,y1)-(x2,y2) của đa giác.
        x1 = xs[n - 1]
        y1 = ys[n - 1]
        for i in range(n):
            x2 = xs[i]
            y2 = ys[i]

            # --- BOUNDARY: điểm có nằm trên cạnh (x1,y1)-(x2,y2) không? ---
            # cross product == 0 nghĩa là ba điểm thẳng hàng; sau đó kiểm tra
            # điểm nằm trong bounding box của cạnh.
            cross = (x2 - x1) * (py - y1) - (y2 - y1) * (px - x1)
            if cross == 0:
                if min(x1, x2) <= px <= max(x1, x2) and \
                   min(y1, y2) <= py <= max(y1, y2):
                    on_boundary = True
                    break

            # --- RAY CASTING: bắn tia về hướng +x và đếm số lần cắt. ---
            # Half-open trick (y1 > py) != (y2 > py) để mỗi đỉnh chỉ đếm một lần.
            if (y1 > py) != (y2 > py):
                dy = y2 - y1
                # Cạnh cắt đường ngang y = py tại hoành độ:
                #   xint = x1 + (x2-x1)*(py-y1)/(y2-y1)
                # Cần biết px < xint (giao điểm nằm bên phải P). Nhân chéo để
                # tránh số thực, đồng thời xét dấu dy.
                lhs = (px - x1) * dy               # (px-x1)*(y2-y1)
                rhs = (x2 - x1) * (py - y1)         # (x2-x1)*(py-y1)
                if dy > 0:
                    if lhs < rhs:
                        inside = not inside
                else:  # dy < 0
                    if lhs > rhs:
                        inside = not inside

            x1 = x2
            y1 = y2

        # Số lần cắt lẻ -> INSIDE, chẵn -> OUTSIDE; ưu tiên BOUNDARY.
        if on_boundary:
            out.append("BOUNDARY")
        elif inside:
            out.append("INSIDE")
        else:
            out.append("OUTSIDE")

    sys.stdout.write("\n".join(out) + "\n")


solve()
