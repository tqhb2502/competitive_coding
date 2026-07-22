#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, int>> movies;
    movies.reserve(n);
    for (int i = 0; i < n; ++i) {
        int start, end;
        cin >> start >> end;
        movies.emplace_back(end, start);
    }

    sort(movies.begin(), movies.end());

    int answer = 0;
    int current_end = -1;
    for (const auto& [end, start] : movies) {
        if (start >= current_end) {
            ++answer;
            current_end = end;
        }
    }

    cout << answer << '\n';
    return 0;
}
