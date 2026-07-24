#include <algorithm>
#include <iostream>
#include <vector>

// Fenwick tree (BIT) lưu số lượng vé còn lại tại mỗi chỉ số giá đã nén.
class FenwickTree {
public:
    // 1. explicit: Từ khóa ngăn chặn trình biên dịch tự động chuyển đổi kiểu dữ liệu 
    // (implicit conversion). Bạn bắt buộc phải khởi tạo cây bằng cú pháp tường minh 
    // như FenwickTree ft(10);
    // 2. Cú pháp Member Initializer List (danh sách khởi tạo thành viên). 
    // Nó khởi tạo vector có tên là tree_ với kích thước là size + 1 và 
    // gán toàn bộ các phần tử ban đầu bằng 0.
    explicit FenwickTree(const int size) : tree_(size + 1, 0) {}

    // Cộng delta vào vị trí position (dùng để thêm hoặc xóa một vé).
    void add(int position, const int delta) {
        while (position < static_cast<int>(tree_.size())) {
            tree_[position] += delta;
            position += position & -position;
        }
    }

    // Tổng tiền tố: số vé còn lại có chỉ số giá <= position.
    int prefix_sum(int position) const {
        int sum = 0;
        while (position > 0) {
            sum += tree_[position];
            position -= position & -position;
        }
        return sum;
    }

    // Binary lifting: trả về vị trí 1-based nhỏ nhất có tổng tiền tố >= order,
    // tức chỉ số của vé đứng thứ hạng order theo thứ tự giá tăng dần.
    int find_by_order(int order) const {
        const int size = static_cast<int>(tree_.size()) - 1;
        int position = 0;
        int step = 1;
        while (step <= size / 2) {
            step *= 2;
        }

        for (; step > 0; step /= 2) {
            const int next = position + step;
            if (next <= size && tree_[next] < order) {
                position = next;
                order -= tree_[next];
            }
        }
        return position + 1;
    }

private:
    std::vector<int> tree_;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int ticket_count;
    int customer_count;
    std::cin >> ticket_count >> customer_count;

    std::vector<int> tickets(ticket_count);
    for (int& price : tickets) {
        std::cin >> price;
    }

    // Nén tọa độ: các mức giá vé phân biệt, sắp tăng dần.
    // 1. Tạo một bản sao của mảng tickets và đặt tên là prices.
    // 2. Sắp xếp toàn bộ các phần tử trong mảng prices theo thứ tự tăng dần.
    // 3. std::unique: Dồn các phần tử trùng lặp liền kề xuống cuối mảng và 
    //    trả về một con trỏ (iterator) chỉ định vị trí bắt đầu của các phần tử thừa đó.
    // 4. Xóa bỏ hoàn toàn các phần tử thừa từ vị trí con trỏ đó cho đến hết mảng.
    std::vector<int> prices = tickets;
    std::sort(prices.begin(), prices.end());
    prices.erase(std::unique(prices.begin(), prices.end()), prices.end());

    // Đếm số vé còn lại tại mỗi mức giá đã nén (1-based).
    FenwickTree remaining(static_cast<int>(prices.size()));
    for (const int price : tickets) {
        const int rank = static_cast<int>(
            std::lower_bound(prices.begin(), prices.end(), price) - prices.begin()) + 1;
        remaining.add(rank, 1);
    }

    for (int customer = 0; customer < customer_count; ++customer) {
        int budget;
        std::cin >> budget;

        // affordable_ranks: số mức giá <= ngân sách (mọi chỉ số 1..affordable_ranks).
        const int affordable_ranks = static_cast<int>(
            std::upper_bound(prices.begin(), prices.end(), budget) - prices.begin());
        // Tổng số vé còn lại có giá <= ngân sách.
        const int affordable_tickets = remaining.prefix_sum(affordable_ranks);

        // Không còn vé phù hợp -> in -1.
        if (affordable_tickets == 0) {
            std::cout << -1 << '\n';
            continue;
        }

        // Vé thứ hạng affordable_tickets là vé giá cao nhất <= ngân sách; bán rồi xóa.
        const int rank = remaining.find_by_order(affordable_tickets);
        std::cout << prices[rank - 1] << '\n';
        remaining.add(rank, -1);
    }

    return 0;
}
