#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> arrivals(n), departures(n);
    for (int i = 0; i < n; ++i) {
        cin >> arrivals[i] >> departures[i];
    }

    sort(arrivals.begin(), arrivals.end());
    sort(departures.begin(), departures.end());

    int arrival_index = 0;
    int departure_index = 0;
    int current = 0;
    int answer = 0;

    while (arrival_index < n) {
        if (arrivals[arrival_index] < departures[departure_index]) {
            ++current;
            answer = max(answer, current);
            ++arrival_index;
        } else {
            // If times are equal, process the departure first: a customer
            // leaving at t does not overlap one arriving at t.
            --current;
            ++departure_index;
        }
    }

    cout << answer << '\n';
    return 0;
}
