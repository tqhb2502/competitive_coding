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

    // SL[i] / SR[i]: núi gần nhất cao hơn i THỰC SỰ ở bên trái / phải
    // (-1 hoặc n nếu không tồn tại). Tính bằng monotonic stack O(n).
    vector<int> nearestGreaterLeft(n, -1);
    vector<int> nearestGreaterRight(n, n);
    vector<int> stack;
    stack.reserve(n);

    // Quét từ trái sang phải tìm cha bên trái.
    for (int i = 0; i < n; ++i) {
        while (!stack.empty() && height[stack.back()] <= height[i]) {
            stack.pop_back();
        }
        if (!stack.empty()) {
            nearestGreaterLeft[i] = stack.back();
        }
        stack.push_back(i);
    }

    // Quét từ phải sang trái tìm cha bên phải.
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

    // Xử lý các núi theo thứ tự chiều cao TĂNG DẦN để mọi đích lượn tới được
    // (đều thấp hơn thực sự) đã có dp trước khi tính dp của núi hiện tại.
    vector<int> order(n);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int first, int second) {
        if (height[first] != height[second]) {
            return height[first] < height[second];
        }
        return first < second;
    });

    // bestChild[i] = max{dp[j]} trên đoạn lượn tới được của i (đóng vai childmax).
    vector<int> bestChild(n, 0);
    int answer = 0;
    for (const int mountain : order) {
        // dp[mountain] = 1 + max dp của các núi lượn tới được.
        const int routeLength = bestChild[mountain] + 1;
        answer = max(answer, routeLength);

        // "Đẩy" dp lên hai cha SL và SR bằng phép max.
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
