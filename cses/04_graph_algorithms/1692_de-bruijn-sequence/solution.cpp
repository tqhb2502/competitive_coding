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

    // Đồ thị De Bruijn: đỉnh là xâu bit độ dài n-1 (2^(n-1) đỉnh).
    // mask giữ lại n-1 bit cuối; n=1 -> mask = 0 (một đỉnh rỗng, hai self-loop).
    const int nodeCount = 1 << (n - 1);
    const int mask = nodeCount - 1;

    // nextBit[node]: cạnh kế tiếp chưa dùng của đỉnh (bit 0 rồi bit 1).
    vector<int> nextBit(nodeCount, 0);
    // Hierholzer lặp bằng stack tường minh (tránh đệ quy sâu).
    vector<int> nodeStack{0};      // ngăn xếp các đỉnh đang duyệt
    vector<int> incomingBit{-1};   // bit của cạnh đã đi vào đỉnh (-1: đỉnh xuất phát)
    vector<int> edgeBits;          // bit các cạnh theo thứ tự kết thúc (ngược của Euler)
    edgeBits.reserve(1 << n);

    while (!nodeStack.empty()) {
        const int node = nodeStack.back();
        if (nextBit[node] < 2) {
            // Còn cạnh chưa dùng: thêm bit b rồi đi tới đỉnh kề.
            const int bit = nextBit[node]++;
            const int nextNode = ((node << 1) | bit) & mask;
            nodeStack.push_back(nextNode);
            incomingBit.push_back(bit);
        } else {
            // Hết cạnh: rút đỉnh khỏi stack và ghi lại bit của cạnh đi vào nó.
            nodeStack.pop_back();
            const int bit = incomingBit.back();
            incomingBit.pop_back();
            if (bit != -1) {
                edgeBits.push_back(bit);
            }
        }
    }

    // Đảo lại để có đúng thứ tự Eulerian circuit.
    reverse(edgeBits.begin(), edgeBits.end());

    // Kết quả: tiền tố n-1 bit '0' của đỉnh xuất phát nối với dãy bit các cạnh.
    string answer(n - 1, '0');
    answer.reserve((1 << n) + n - 1);
    for (const int bit : edgeBits) {
        answer.push_back(static_cast<char>('0' + bit));
    }
    cout << answer << '\n';
    return 0;
}
