#include <algorithm>
#include <cstdint>
#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int size = 0;
    cin >> size;
    vector<long long> values(size);
    for (long long& value : values) {
        cin >> value;
    }
    // Sắp xếp tăng dần: nghiệm tối ưu chỉ cần ghép hai phần tử kề nhau.
    sort(values.begin(), values.end());

    // Xem như đường đi gồm size điểm với size-1 cạnh thật: cạnh i (1 <= i < size)
    // có trọng số values[i] - values[i-1].
    // Hai cạnh biên giả (0 và size) mang chi phí vô cùng để chặn ngoài biên.
    constexpr long long INFINITY_COST = 3'000'000'000'000'000'000LL;
    vector<long long> cost(size + 1, INFINITY_COST);
    vector<int> left(size + 1, 0);    // Cạnh sống liền trước trong danh sách liên kết.
    vector<int> right(size + 1, size); // Cạnh sống liền sau trong danh sách liên kết.
    vector<int> version(size + 1, 0);  // Phiên bản để bỏ qua bản ghi cũ trong heap.
    vector<char> alive(size + 1, true);

    using Entry = tuple<long long, int, int>;
    priority_queue<Entry, vector<Entry>, greater<Entry>> queue;

    // Khởi tạo con trỏ liên kết đôi cho mọi cạnh.
    for (int edge = 0; edge <= size; ++edge) {
        left[edge] = max(0, edge - 1);
        right[edge] = min(size, edge + 1);
    }
    // Đưa trọng số các cạnh thật vào heap.
    for (int edge = 1; edge < size; ++edge) {
        cost[edge] = values[edge] - values[edge - 1];
        queue.emplace(cost[edge], edge, version[edge]);
    }

    long long totalCost = 0;
    // Lần lượt tăng số cặp k; mỗi bước tham lam lấy cạnh nhỏ nhất còn hợp lệ.
    for (int pairCount = 1; pairCount <= size / 2; ++pairCount) {
        Entry best{};
        // Bỏ các bản ghi đã lỗi thời (chết hoặc sai phiên bản / sai trọng số).
        while (true) {
            best = queue.top();
            queue.pop();
            const auto [storedCost, edge, storedVersion] = best;
            if (alive[edge] && storedVersion == version[edge] &&
                storedCost == cost[edge]) {
                break;
            }
        }

        const auto [chosenCost, edge, storedVersion] = best;
        static_cast<void>(storedVersion);
        // Cộng chi phí biên của bước này -> đáp án tối ưu cho pairCount cặp.
        totalCost += chosenCost;
        cout << totalCost << (pairCount == size / 2 ? '\n' : ' ');

        // Hai cạnh kề bị xóa vì xung đột đỉnh với cạnh vừa chọn.
        const int leftEdge = left[edge];
        const int rightEdge = right[edge];
        const int outsideLeft = left[leftEdge];
        const int outsideRight = right[rightEdge];

        alive[leftEdge] = false;
        alive[rightEdge] = false;
        // Tạo cạnh co "hối tiếc": chọn nó sau này = bỏ w, thay bằng cả l và r.
        cost[edge] = cost[leftEdge] + cost[rightEdge] - cost[edge];
        ++version[edge];

        // Nối lại danh sách liên kết, bỏ hai cạnh kề vừa xóa ra khỏi đường đi.
        left[edge] = outsideLeft;
        right[edge] = outsideRight;
        right[outsideLeft] = edge;
        left[outsideRight] = edge;
        queue.emplace(cost[edge], edge, version[edge]);
    }
}
