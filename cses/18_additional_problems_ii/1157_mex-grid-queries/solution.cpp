#include <cstdint>
#include <iostream>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int64_t row = 0;
    int64_t column = 0;
    cin >> row >> column;
    cout << ((row - 1) ^ (column - 1)) << '\n';
}
