#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Đọc functional graph: successor[i] là hành tinh mà i dịch chuyển tới.
    int n;
    cin >> n;
    vector<int> successor(n + 1);
    for (int planet = 1; planet <= n; ++planet) {
        cin >> successor[planet];
    }

    // state: 0 = chưa thăm, 1 = đang nằm trong chuỗi hiện tại, 2 = đã tính xong.
    // position: vị trí của đỉnh trong chuỗi (chain) hiện tại.
    vector<int> state(n + 1, 0);
    vector<int> position(n + 1, 0);
    vector<int> answer(n + 1, 0);

    for (int start = 1; start <= n; ++start) {
        if (state[start] != 0) {
            continue;
        }

        // Đi theo cổng dịch chuyển, đẩy các đỉnh vào chain tới khi gặp đỉnh biên.
        vector<int> chain;
        int planet = start;
        while (state[planet] == 0) {
            state[planet] = 1;
            position[planet] = static_cast<int>(chain.size());
            chain.push_back(planet);
            planet = successor[planet];
        }

        if (state[planet] == 1) {
            // Phát hiện chu trình mới nằm ngay trong chain hiện tại.
            const int cycleStart = position[planet];
            const int cycleLength = static_cast<int>(chain.size()) - cycleStart;
            // Mọi đỉnh trên chu trình có đáp án = độ dài chu trình.
            for (int i = cycleStart; i < static_cast<int>(chain.size()); ++i) {
                answer[chain[i]] = cycleLength;
                state[chain[i]] = 2;
            }

            // Các đỉnh đuôi phía trước: đáp án tăng dần khi đi ngược lại chain.
            int length = cycleLength;
            for (int i = cycleStart - 1; i >= 0; --i) {
                answer[chain[i]] = ++length;
                state[chain[i]] = 2;
            }
        } else {
            // state[planet] == 2: toàn bộ chain là đuôi dẫn vào phần đã xử lý.
            int length = answer[planet];
            for (int i = static_cast<int>(chain.size()) - 1; i >= 0; --i) {
                answer[chain[i]] = ++length;
                state[chain[i]] = 2;
            }
        }
    }

    // In đáp án cho từng hành tinh, cách nhau bởi dấu cách.
    for (int planet = 1; planet <= n; ++planet) {
        if (planet > 1) {
            cout << ' ';
        }
        cout << answer[planet];
    }
    cout << '\n';
    return 0;
}
