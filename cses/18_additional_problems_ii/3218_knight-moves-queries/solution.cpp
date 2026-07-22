#include <algorithm>
#include <iostream>

using namespace std;

using Integer = long long;

Integer knightDistance(Integer first, Integer second) {
    if (first < second) {
        swap(first, second);
    }
    if (first == 1 && second == 0) {
        return 3;
    }
    if (first == 1 && second == 1) {
        return 4;
    }
    if (first == 2 && second == 2) {
        return 4;
    }
    Integer moves = max((first + 1) / 2, (first + second + 2) / 3);
    if ((moves + first + second) % 2 != 0) {
        ++moves;
    }
    return moves;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int queries;
    cin >> queries;
    while (queries-- > 0) {
        Integer row;
        Integer column;
        cin >> row >> column;
        cout << knightDistance(row - 1, column - 1) << '\n';
    }
}
