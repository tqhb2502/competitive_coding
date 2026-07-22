#include <iostream>
#include <utility>
#include <vector>

using namespace std;

void hanoi(int disks, int source, int destination, int auxiliary,
           vector<pair<int, int>>& moves) {
    if (disks == 0) {
        return;
    }

    hanoi(disks - 1, source, auxiliary, destination, moves);
    moves.emplace_back(source, destination);
    hanoi(disks - 1, auxiliary, destination, source, moves);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int disks;
    cin >> disks;

    vector<pair<int, int>> moves;
    moves.reserve((1U << disks) - 1);
    hanoi(disks, 1, 3, 2, moves);

    cout << moves.size() << '\n';
    for (const auto& [source, destination] : moves) {
        cout << source << ' ' << destination << '\n';
    }
    return 0;
}
