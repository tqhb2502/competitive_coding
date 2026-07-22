#include <array>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    const int width = n + 4;
    vector<int> distance(width * width, -2);
    for (int row = 0; row < n; ++row) {
        const int first = (row + 2) * width + 2;
        for (int col = 0; col < n; ++col) {
            distance[first + col] = -1;
        }
    }

    const int start = 2 * width + 2;
    distance[start] = 0;
    const array<int, 8> offsets = {
        width + 2, width - 2, -width + 2, -width - 2,
        2 * width + 1, 2 * width - 1, -2 * width + 1, -2 * width - 1
    };

    vector<int> queue;
    queue.reserve(n * n);
    queue.push_back(start);
    for (size_t head = 0; head < queue.size(); ++head) {
        const int current = queue[head];
        for (const int offset : offsets) {
            const int next = current + offset;
            if (distance[next] == -1) {
                distance[next] = distance[current] + 1;
                queue.push_back(next);
            }
        }
    }

    for (int row = 0; row < n; ++row) {
        const int first = (row + 2) * width + 2;
        for (int col = 0; col < n; ++col) {
            if (col > 0) {
                cout << ' ';
            }
            cout << distance[first + col];
        }
        cout << '\n';
    }

    return 0;
}
