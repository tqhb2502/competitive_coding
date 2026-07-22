#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long target;
    cin >> n >> target;

    vector<long long> machine_times(n);
    for (long long& time : machine_times) {
        cin >> time;
    }

    long long low = 0;
    long long high = *min_element(machine_times.begin(), machine_times.end()) * target;

    while (low < high) {
        const long long middle = low + (high - low) / 2;
        long long products = 0;

        for (const long long time : machine_times) {
            products += middle / time;
            if (products >= target) {
                break;
            }
        }

        if (products >= target) {
            high = middle;
        } else {
            low = middle + 1;
        }
    }

    cout << low << '\n';
    return 0;
}
