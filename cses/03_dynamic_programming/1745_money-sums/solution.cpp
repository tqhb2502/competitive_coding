#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> coins(n);
    int maximum_sum = 0;
    for (int& coin : coins) {
        cin >> coin;
        maximum_sum += coin;
    }

    vector<char> possible(maximum_sum + 1, false);
    possible[0] = true;
    int processed_sum = 0;
    for (const int coin : coins) {
        for (int sum = processed_sum; sum >= 0; --sum) {
            if (possible[sum]) {
                possible[sum + coin] = true;
            }
        }
        processed_sum += coin;
    }

    vector<int> answers;
    for (int sum = 1; sum <= maximum_sum; ++sum) {
        if (possible[sum]) {
            answers.push_back(sum);
        }
    }

    cout << answers.size() << '\n';
    for (size_t i = 0; i < answers.size(); ++i) {
        if (i != 0) {
            cout << ' ';
        }
        cout << answers[i];
    }
    cout << '\n';
    return 0;
}
