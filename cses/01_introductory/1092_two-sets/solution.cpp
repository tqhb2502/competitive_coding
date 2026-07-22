#include <iostream>
#include <vector>

using namespace std;

void print_set(const vector<int>& values) {
    cout << values.size() << '\n';
    for (size_t i = 0; i < values.size(); ++i) {
        if (i > 0) {
            cout << ' ';
        }
        cout << values[i];
    }
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    if (n % 4 != 0 && n % 4 != 3) {
        cout << "NO\n";
        return 0;
    }

    const long long target = 1LL * n * (n + 1) / 4;
    long long remaining = target;
    vector<int> first_set;
    vector<int> second_set;

    for (int value = n; value >= 1; --value) {
        if (value <= remaining) {
            first_set.push_back(value);
            remaining -= value;
        } else {
            second_set.push_back(value);
        }
    }

    cout << "YES\n";
    print_set(first_set);
    print_set(second_set);
    return 0;
}
