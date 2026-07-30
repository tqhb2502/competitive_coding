#include <algorithm>
#include <deque>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int bookCount = 0;
    int budget = 0;
    cin >> bookCount >> budget;

    vector<int> price(bookCount);
    vector<int> pages(bookCount);
    vector<int> copies(bookCount);
    for (int& value : price) {
        cin >> value;
    }
    for (int& value : pages) {
        cin >> value;
    }
    for (int& value : copies) {
        cin >> value;
    }

    // dp[c] = số trang lớn nhất khi dùng ngân sách không quá c.
    vector<int> previous(budget + 1, 0);
    vector<int> current(budget + 1, 0);

    // Bounded knapsack: xử lý lần lượt từng loại sách.
    for (int book = 0; book < bookCount; ++book) {
        const int cost = price[book];   // giá của loại sách
        const int value = pages[book];  // số trang của loại sách
        const int limit = copies[book]; // giới hạn số bản được mua

        // Xét riêng từng lớp đồng dư r theo modulo cost.
        for (int remainder = 0; remainder < cost && remainder <= budget;
             ++remainder) {
            // Deque đơn điệu giữ các chỉ số j có dp[r+j*cost]-j*value giảm dần.
            deque<int> candidates;
            int quotient = 0;
            for (int money = remainder; money <= budget;
                 money += cost, ++quotient) {
                // Bỏ các chỉ số đã rơi ra khỏi cửa sổ trượt [quotient-limit, quotient].
                while (!candidates.empty() &&
                       candidates.front() < quotient - limit) {
                    candidates.pop_front();
                }

                // Đẩy trạng thái hiện tại vào và giữ tính đơn điệu của deque.
                const int transformed = previous[money] - quotient * value;
                while (!candidates.empty()) {
                    const int lastQuotient = candidates.back();
                    const int lastMoney = remainder + lastQuotient * cost;
                    if (previous[lastMoney] - lastQuotient * value >=
                        transformed) {
                        break;
                    }
                    candidates.pop_back();
                }
                candidates.push_back(quotient);

                // Phần tử đầu deque cho phương án tốt nhất trong cửa sổ.
                const int bestQuotient = candidates.front();
                const int bestMoney = remainder + bestQuotient * cost;
                current[money] = previous[bestMoney] +
                                 (quotient - bestQuotient) * value;
            }
        }
        previous.swap(current);
    }

    cout << previous[budget] << '\n';
}
