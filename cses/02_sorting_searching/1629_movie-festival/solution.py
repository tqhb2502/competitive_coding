# Movie Festival - https://cses.fi/problemset/task/1629
# Greedy interval scheduling: sort movies by ending time, then pick a movie
# whenever its start time is >= the end time of the last chosen movie.

import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return

    idx = 0
    n = int(data[idx]); idx += 1

    movies = []
    for _ in range(n):
        a = int(data[idx]); idx += 1
        b = int(data[idx]); idx += 1
        # Store (end, start) so sorting by end time is natural.
        movies.append((b, a))

    movies.sort()

    count = 0
    cur_end = -1  # end time of the last chosen movie (times are >= 1)
    for end, start in movies:
        if start >= cur_end:
            count += 1
            cur_end = end

    sys.stdout.write(str(count) + "\n")


if __name__ == "__main__":
    main()
