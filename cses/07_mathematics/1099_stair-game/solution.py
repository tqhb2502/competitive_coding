import sys


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0
    t = int(data[pos]); pos += 1
    out = []
    for _ in range(t):
        n = int(data[pos]); pos += 1
        # Staircase Nim: XOR số bóng trên các bậc 1-indexed CHẴN, tức các
        # chỉ số 0-based LẺ (offset+1, offset+3, ...) trong mảng của test này.
        x = 0
        for j in range(pos + 1, pos + n, 2):
            x ^= int(data[j])
        pos += n
        # XOR khác 0 -> người đi trước thắng, ngược lại người đi sau thắng.
        out.append("first" if x != 0 else "second")
    sys.stdout.write("\n".join(out) + "\n")


main()
