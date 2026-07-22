#include <functional>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long totalLength = 0;
    int pieceCount = 0;
    cin >> totalLength >> pieceCount;
    static_cast<void>(totalLength);

    priority_queue<long long, vector<long long>, greater<long long>> pieces;
    for (int index = 0; index < pieceCount; ++index) {
        long long length = 0;
        cin >> length;
        pieces.push(length);
    }

    long long answer = 0;
    while (pieces.size() > 1U) {
        const long long first = pieces.top();
        pieces.pop();
        const long long second = pieces.top();
        pieces.pop();
        answer += first + second;
        pieces.push(first + second);
    }

    cout << answer << '\n';
}
