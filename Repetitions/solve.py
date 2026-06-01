import sys


def solve():
    input = sys.stdin.readline
    text = input()

    maxlen = 1
    count = 1
    prev_char = text[0]

    for char in text[1:]:
        if char == prev_char:
            count += 1
        else:
            count = 1
        
        maxlen = max(maxlen, count)
        prev_char = char

    print(maxlen)


if __name__ == "__main__":
    solve()
