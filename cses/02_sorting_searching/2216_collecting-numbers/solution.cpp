#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> position(n + 1);
    for (int i = 0; i < n; ++i) {
        int value;
        cin >> value;
        position[value] = i;
    }

    int rounds = 1;
    for (int value = 2; value <= n; ++value) {
        if (position[value] < position[value - 1]) {
            ++rounds;
        }
    }

    cout << rounds << '\n';
    return 0;
}
