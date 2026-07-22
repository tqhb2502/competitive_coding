#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

using Integer = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int children;
    cin >> children;
    vector<Integer> difference(static_cast<size_t>(children));
    for (Integer &value : difference) {
        cin >> value;
    }
    for (Integer &value : difference) {
        Integer wanted;
        cin >> wanted;
        value -= wanted;
    }

    vector<Integer> prefix(static_cast<size_t>(children));
    Integer running = 0;
    for (int index = 0; index < children; ++index) {
        running += difference[static_cast<size_t>(index)];
        prefix[static_cast<size_t>(index)] = running;
    }
    nth_element(prefix.begin(), prefix.begin() + children / 2, prefix.end());
    const Integer median = prefix[static_cast<size_t>(children / 2)];
    Integer answer = 0;
    for (const Integer value : prefix) {
        answer += llabs(value - median);
    }
    cout << answer << '\n';
}
