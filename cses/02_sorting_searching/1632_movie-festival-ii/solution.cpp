#include <algorithm>
#include <iostream>
#include <set>
#include <utility>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<pair<int, int>> movies;
    movies.reserve(n);
    for (int i = 0; i < n; ++i) {
        int start, end;
        cin >> start >> end;
        movies.emplace_back(end, start);
    }

    sort(movies.begin(), movies.end());

    multiset<int> available_times;
    for (int member = 0; member < k; ++member) {
        available_times.insert(0);
    }

    int answer = 0;
    for (const auto& [end, start] : movies) {
        auto member = available_times.upper_bound(start);
        if (member == available_times.begin()) {
            continue;
        }

        --member;
        available_times.erase(member);
        available_times.insert(end);
        ++answer;
    }

    cout << answer << '\n';
    return 0;
}
