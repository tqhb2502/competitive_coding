#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<long long> coins(n);
    for (long long& coin : coins) {
        cin >> coin;
    }
    sort(coins.begin(), coins.end());

    long long smallestMissing = 1;
    for (const long long coin : coins) {
        if (coin > smallestMissing) {
            break;
        }
        smallestMissing += coin;
    }

    cout << smallestMissing << '\n';
    return 0;
}
