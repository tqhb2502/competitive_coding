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

    vector<pair<long long, long long>> tasks(n);
    for (auto& [duration, deadline] : tasks) {
        cin >> duration >> deadline;
    }

    sort(tasks.begin(), tasks.end());

    long long current_time = 0;
    long long reward = 0;
    for (const auto& [duration, deadline] : tasks) {
        current_time += duration;
        reward += deadline - current_time;
    }

    cout << reward << '\n';
    return 0;
}
