#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> height(n);
    for (int& value : height) {
        cin >> value;
    }

    vector<int> nearestGreaterLeft(n, -1);
    vector<int> nearestGreaterRight(n, n);
    vector<int> stack;
    stack.reserve(n);

    for (int i = 0; i < n; ++i) {
        while (!stack.empty() && height[stack.back()] <= height[i]) {
            stack.pop_back();
        }
        if (!stack.empty()) {
            nearestGreaterLeft[i] = stack.back();
        }
        stack.push_back(i);
    }

    stack.clear();
    for (int i = n - 1; i >= 0; --i) {
        while (!stack.empty() && height[stack.back()] <= height[i]) {
            stack.pop_back();
        }
        if (!stack.empty()) {
            nearestGreaterRight[i] = stack.back();
        }
        stack.push_back(i);
    }

    vector<int> order(n);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int first, int second) {
        if (height[first] != height[second]) {
            return height[first] < height[second];
        }
        return first < second;
    });

    vector<int> bestChild(n, 0);
    int answer = 0;
    for (const int mountain : order) {
        const int routeLength = bestChild[mountain] + 1;
        answer = max(answer, routeLength);

        const int leftParent = nearestGreaterLeft[mountain];
        if (leftParent != -1) {
            bestChild[leftParent] = max(bestChild[leftParent], routeLength);
        }
        const int rightParent = nearestGreaterRight[mountain];
        if (rightParent != n) {
            bestChild[rightParent] = max(bestChild[rightParent], routeLength);
        }
    }

    cout << answer << '\n';
    return 0;
}
