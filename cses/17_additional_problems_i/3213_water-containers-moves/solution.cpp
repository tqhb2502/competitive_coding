#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <limits>
#include <numeric>
#include <queue>
#include <string>
#include <utility>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int capacityA = 0;
    int capacityB = 0;
    int target = 0;
    std::cin >> capacityA >> capacityB >> target;

    // Điều kiện cần và đủ để có lời giải: đích không vượt dung tích bình A và
    // là bội của gcd(a, b) (định lý hai bình).
    if (target > capacityA || target % std::gcd(capacityA, capacityB) != 0) {
        std::cout << "-1\n";
        return 0;
    }

    // Mã hóa trạng thái (lượng nước trong A, lượng nước trong B) thành một số.
    const int width = capacityB + 1;
    const int stateCount = (capacityA + 1) * width;
    const auto encode = [width](int amountA, int amountB) {
        return amountA * width + amountB;
    };

    // Dijkstra: distance = tổng lượng nước đã di chuyển nhỏ nhất tới trạng thái;
    // parent / parentOperation dùng để lần ngược dựng lại dãy thao tác.
    constexpr long long INF = std::numeric_limits<long long>::max() / 4;
    std::vector<long long> distance(static_cast<std::size_t>(stateCount), INF);
    std::vector<int> parent(static_cast<std::size_t>(stateCount), -1);
    std::vector<unsigned char> parentOperation(static_cast<std::size_t>(stateCount), 0);
    using QueueItem = std::pair<long long, int>;
    std::priority_queue<QueueItem, std::vector<QueueItem>, std::greater<>> queue;

    const int start = encode(0, 0);
    distance[static_cast<std::size_t>(start)] = 0;
    queue.emplace(0, start);
    int finish = -1;

    while (!queue.empty()) {
        const QueueItem current = queue.top();
        queue.pop();
        const long long currentDistance = current.first;
        const int state = current.second;
        // Bỏ qua bản ghi cũ đã bị cập nhật tốt hơn.
        if (currentDistance != distance[static_cast<std::size_t>(state)]) {
            continue;
        }
        const int amountA = state / width;
        const int amountB = state % width;
        // Lấy ra trạng thái có A = x đầu tiên: khoảng cách của nó là tối ưu.
        if (amountA == target) {
            finish = state;
            break;
        }

        // Cập nhật (relax) một cạnh với chi phí = lượng nước thực sự di chuyển;
        // bỏ qua thao tác không di chuyển đơn vị nào.
        const auto relax = [&](int nextA, int nextB, int moved, unsigned char operation) {
            if (moved == 0) {
                return;
            }
            const int next = encode(nextA, nextB);
            const long long candidate = currentDistance + moved;
            if (candidate < distance[static_cast<std::size_t>(next)]) {
                distance[static_cast<std::size_t>(next)] = candidate;
                parent[static_cast<std::size_t>(next)] = state;
                parentOperation[static_cast<std::size_t>(next)] = operation;
                queue.emplace(candidate, next);
            }
        };

        // Sáu thao tác hợp lệ: đổ đầy A/B, đổ bỏ A/B, rót A sang B và B sang A.
        relax(capacityA, amountB, capacityA - amountA, 0);
        relax(amountA, capacityB, capacityB - amountB, 1);
        relax(0, amountB, amountA, 2);
        relax(amountA, 0, amountB, 3);
        const int fromAToB = std::min(amountA, capacityB - amountB);
        relax(amountA - fromAToB, amountB + fromAToB, fromAToB, 4);
        const int fromBToA = std::min(amountB, capacityA - amountA);
        relax(amountA + fromBToA, amountB - fromBToA, fromBToA, 5);
    }

    if (finish == -1) {
        std::cout << "-1\n";
        return 0;
    }

    // Lần ngược theo cha từ trạng thái đích về gốc để lấy dãy thao tác.
    std::vector<unsigned char> moves;
    for (int state = finish; state != start; state = parent[static_cast<std::size_t>(state)]) {
        moves.push_back(parentOperation[static_cast<std::size_t>(state)]);
    }
    std::reverse(moves.begin(), moves.end());

    // In số bước, tổng lượng nước di chuyển, rồi tên từng thao tác theo thứ tự.
    const std::array<std::string, 6> operationName{
        "FILL A", "FILL B", "EMPTY A", "EMPTY B", "MOVE A B", "MOVE B A"};
    std::cout << moves.size() << ' ' << distance[static_cast<std::size_t>(finish)] << '\n';
    for (const unsigned char operation : moves) {
        std::cout << operationName[static_cast<std::size_t>(operation)] << '\n';
    }
    return 0;
}
