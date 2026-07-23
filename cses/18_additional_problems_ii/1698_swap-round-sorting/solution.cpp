#include <iostream>
#include <utility>
#include <vector>

using namespace std;

using Swap = pair<int, int>;

// In ra một vòng: số cặp hoán đổi, rồi từng cặp chỉ số (đổi về 1-based)
void printRound(const vector<Swap> &swaps) {
    cout << swaps.size() << '\n';
    for (const auto [first, second] : swaps) {
        cout << first + 1 << ' ' << second + 1 << '\n';
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int size;
    cin >> size;
    vector<int> permutation(static_cast<size_t>(size));
    for (int &value : permutation) {
        cin >> value;
        --value; // Chuyển sang chỉ số 0-based
    }

    vector<char> visited(static_cast<size_t>(size), false);
    vector<Swap> firstRound;
    vector<Swap> secondRound;
    for (int start = 0; start < size; ++start) {
        if (visited[static_cast<size_t>(start)]) {
            continue;
        }
        // Duyệt lần theo chu trình chứa vị trí start
        vector<int> cycle;
        int current = start;
        while (!visited[static_cast<size_t>(current)]) {
            visited[static_cast<size_t>(current)] = true;
            cycle.push_back(current);
            current = permutation[static_cast<size_t>(current)];
        }

        // Vòng 1: các cặp đối xứng (c[left], c[right]) từ hai đầu vào giữa
        int left = 0;
        int right = static_cast<int>(cycle.size()) - 1;
        while (left < right) {
            firstRound.emplace_back(cycle[static_cast<size_t>(left)],
                                    cycle[static_cast<size_t>(right)]);
            ++left;
            --right;
        }
        // Vòng 2: các cặp đối xứng bắt đầu từ i = 1 (cần khi chu trình dài >= 3)
        left = 1;
        right = static_cast<int>(cycle.size()) - 1;
        while (left < right) {
            secondRound.emplace_back(cycle[static_cast<size_t>(left)],
                                     cycle[static_cast<size_t>(right)]);
            ++left;
            --right;
        }
    }

    // Chọn đáp án: 0 vòng nếu đã sắp; 1 vòng nếu toàn chu trình ngắn; ngược lại 2 vòng
    if (firstRound.empty()) {
        cout << 0 << '\n';
    } else if (secondRound.empty()) {
        cout << 1 << '\n';
        printRound(firstRound);
    } else {
        cout << 2 << '\n';
        printRound(firstRound);
        printRound(secondRound);
    }
}
