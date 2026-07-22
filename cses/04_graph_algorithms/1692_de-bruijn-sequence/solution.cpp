#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    const int nodeCount = 1 << (n - 1);
    const int mask = nodeCount - 1;
    vector<int> nextBit(nodeCount, 0);
    vector<int> nodeStack{0};
    vector<int> incomingBit{-1};
    vector<int> edgeBits;
    edgeBits.reserve(1 << n);

    while (!nodeStack.empty()) {
        const int node = nodeStack.back();
        if (nextBit[node] < 2) {
            const int bit = nextBit[node]++;
            const int nextNode = ((node << 1) | bit) & mask;
            nodeStack.push_back(nextNode);
            incomingBit.push_back(bit);
        } else {
            nodeStack.pop_back();
            const int bit = incomingBit.back();
            incomingBit.pop_back();
            if (bit != -1) {
                edgeBits.push_back(bit);
            }
        }
    }

    reverse(edgeBits.begin(), edgeBits.end());
    string answer(n - 1, '0');
    answer.reserve((1 << n) + n - 1);
    for (const int bit : edgeBits) {
        answer.push_back(static_cast<char>('0' + bit));
    }
    cout << answer << '\n';
    return 0;
}
