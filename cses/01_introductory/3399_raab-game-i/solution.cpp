#include <iostream>
#include <vector>

using namespace std;

void printPermutation(const vector<int>& permutation) {
    for (size_t i = 0; i < permutation.size(); ++i) {
        if (i > 0) {
            cout << ' ';
        }
        cout << permutation[i];
    }
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tests;
    cin >> tests;
    while (tests--) {
        int n, a, b;
        cin >> n >> a >> b;

        if (a + b > n || ((a == 0) != (b == 0))) {
            cout << "NO\n";
            continue;
        }

        const int moved = a + b;
        vector<int> firstPlayer;
        vector<int> secondPlayer;
        firstPlayer.reserve(n);
        secondPlayer.reserve(n);

        for (int card = b + 1; card <= moved; ++card) {
            firstPlayer.push_back(card);
        }
        for (int card = 1; card <= b; ++card) {
            firstPlayer.push_back(card);
        }
        for (int card = moved + 1; card <= n; ++card) {
            firstPlayer.push_back(card);
        }
        for (int card = 1; card <= n; ++card) {
            secondPlayer.push_back(card);
        }

        cout << "YES\n";
        printPermutation(firstPlayer);
        printPermutation(secondPlayer);
    }

    return 0;
}
