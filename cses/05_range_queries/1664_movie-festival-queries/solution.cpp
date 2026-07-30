#include <algorithm>
#include <iostream>
#include <limits>
#include <utility>
#include <vector>

using namespace std;

static size_t to_index(int value) {
    return static_cast<size_t>(value);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    // Đọc n phim; đồng thời gom các giá trị end để node hóa về sau.
    vector<pair<int, int>> movies(to_index(n));
    vector<int> distinct_ends;
    distinct_ends.reserve(to_index(n));
    for (auto &[start, finish] : movies) {
        cin >> start >> finish;
        distinct_ends.push_back(finish);
    }

    // Sắp xếp phim theo start tăng dần để chuẩn bị suffix-min của end.
    sort(movies.begin(), movies.end());

    vector<int> starts(to_index(n));
    vector<int> suffix_minimum(to_index(n + 1), numeric_limits<int>::max());
    for (int i = 0; i < n; ++i) {
        starts[to_index(i)] = movies[to_index(i)].first;
    }
    // suffix_minimum[i] = end nhỏ nhất trong các phim có chỉ số >= i.
    // Kết hợp với lower_bound trên starts cho ta f(t) = min end với start >= t.
    for (int i = n - 1; i >= 0; --i) {
        suffix_minimum[to_index(i)] = min(suffix_minimum[to_index(i + 1)],
                                          movies[to_index(i)].second);
    }

    // Node hóa tập các giá trị end phân biệt; state_count = số node,
    // sentinel = trạng thái "không còn phim nào".
    sort(distinct_ends.begin(), distinct_ends.end());
    distinct_ends.erase(unique(distinct_ends.begin(), distinct_ends.end()),
                        distinct_ends.end());
    const int state_count = static_cast<int>(distinct_ends.size());
    const int sentinel = state_count;

    vector<int> state_value = distinct_ends;
    state_value.push_back(numeric_limits<int>::max());

    // Số tầng binary lifting đủ để bao mọi đáp án <= n.
    int levels = 1;
    while ((1LL << levels) <= n) {
        ++levels;
    }
    vector<vector<int>> jump(to_index(levels),
                             vector<int>(to_index(state_count + 1), sentinel));

    // jump[0][state] = node của f(giá trị end tại state); sentinel nếu hết phim.
    for (int state = 0; state < state_count; ++state) {
        const int current_time = distinct_ends[to_index(state)];
        const int movie_index = static_cast<int>(
            lower_bound(starts.begin(), starts.end(), current_time) - starts.begin());
        const int next_end = suffix_minimum[to_index(movie_index)];
        if (next_end != numeric_limits<int>::max()) {
            jump[0][to_index(state)] = static_cast<int>(
                lower_bound(distinct_ends.begin(), distinct_ends.end(), next_end)
                - distinct_ends.begin());
        }
    }
    // Ghép đôi các bước nhảy: jump[k][i] = trạng thái sau 2^k bước.
    for (int level = 1; level < levels; ++level) {
        for (int state = 0; state <= state_count; ++state) {
            const int middle = jump[to_index(level - 1)][to_index(state)];
            jump[to_index(level)][to_index(state)] =
                jump[to_index(level - 1)][to_index(middle)];
        }
    }

    while (q--) {
        int arrival, departure;
        cin >> arrival >> departure;

        // Phim đầu tiên: f(a). Nếu end nhỏ nhất đã vượt b thì không xem được.
        const int movie_index = static_cast<int>(
            lower_bound(starts.begin(), starts.end(), arrival) - starts.begin());
        const int first_end = suffix_minimum[to_index(movie_index)];
        if (first_end > departure) {
            cout << 0 << '\n';
            continue;
        }

        // Đã xem 1 phim; dùng binary lifting để nhảy tối đa số bước còn hợp lệ.
        int answer = 1;
        int state = static_cast<int>(
            lower_bound(distinct_ends.begin(), distinct_ends.end(), first_end)
            - distinct_ends.begin());
        for (int level = levels - 1; level >= 0; --level) {
            const int next_state = jump[to_index(level)][to_index(state)];
            if (state_value[to_index(next_state)] <= departure) {
                answer += 1 << level;
                state = next_state;
            }
        }
        cout << answer << '\n';
    }

    return 0;
}
