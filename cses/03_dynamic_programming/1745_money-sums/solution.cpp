#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Đọc n đồng xu và tính tổng lớn nhất có thể (maxSum)
    int n;
    cin >> n;
    vector<int> coins(n);
    int maximum_sum = 0;
    for (int& coin : coins) {
        cin >> coin;
        maximum_sum += coin;
    }

    // DP subset-sum: possible[s] = tổng s có tạo được từ tập con đã xét không
    // Cơ sở: tập rỗng cho tổng 0
    vector<char> possible(maximum_sum + 1, false);
    possible[0] = true;

    // 0/1 knapsack: mỗi đồng xu xét đúng một lần
    // processed_sum là tổng các đồng xu đã xét, giới hạn phạm vi cần duyệt
    int processed_sum = 0;
    for (const int coin : coins) {
        // Duyệt giảm dần để mỗi đồng xu chỉ được dùng một lần
        for (int sum = processed_sum; sum >= 0; --sum) {
            if (possible[sum]) {
                possible[sum + coin] = true;
            }
        }
        processed_sum += coin;
    }

    // Thu thập mọi tổng khả thi trong [1, maxSum] theo thứ tự tăng dần
    vector<int> answers;
    for (int sum = 1; sum <= maximum_sum; ++sum) {
        if (possible[sum]) {
            answers.push_back(sum);
        }
    }

    // In số lượng tổng khác nhau rồi liệt kê các tổng đó
    cout << answers.size() << '\n';
    for (size_t i = 0; i < answers.size(); ++i) {
        if (i != 0) {
            cout << ' ';
        }
        cout << answers[i];
    }
    cout << '\n';
    return 0;
}
