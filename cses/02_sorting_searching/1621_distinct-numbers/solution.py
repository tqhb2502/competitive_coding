# Distinct Numbers - https://cses.fi/problemset/task/1621
# Count the number of distinct values in a list of n integers.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    n = int(data[0])
    # Lấy đúng n số sau token đầu tiên và đưa vào set để đếm giá trị phân biệt.
    distinct = len(set(data[1:1 + n]))
    sys.stdout.write(str(distinct) + "\n")


main()
