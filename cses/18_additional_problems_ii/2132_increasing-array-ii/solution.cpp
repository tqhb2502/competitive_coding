#include <iostream>
#include <queue>
#include <vector>

using namespace std;

using Integer = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int size;
    cin >> size;
    priority_queue<Integer> levels;
    Integer answer = 0;
    for (int index = 0; index < size; ++index) {
        Integer value;
        cin >> value;
        levels.push(value);
        if (levels.top() > value) {
            answer += levels.top() - value;
            levels.pop();
            levels.push(value);
        }
    }
    cout << answer << '\n';
}
