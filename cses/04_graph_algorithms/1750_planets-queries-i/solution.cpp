#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, queries;
    cin >> n >> queries;

    // Binary lifting: successor[level][v] = đỉnh đến được sau 2^level bước từ v.
    // Cần 2^LEVELS > k_max = 1e9, chọn LEVELS = 30.
    constexpr int LEVELS = 30;
    vector<vector<int>> successor(LEVELS, vector<int>(n + 1));

    // successor[0][v] = t[v]: đích của teleporter duy nhất tại mỗi hành tinh.
    for (int planet = 1; planet <= n; ++planet) {
        cin >> successor[0][planet];
    }

    // Dựng bảng theo quy nạp: đi 2^level = ghép hai lần đi 2^(level-1).
    for (int level = 1; level < LEVELS; ++level) {
        for (int planet = 1; planet <= n; ++planet) {
            successor[level][planet] =
                successor[level - 1][successor[level - 1][planet]];
        }
    }

    // Trả lời từng truy vấn: phân tích steps theo nhị phân, mỗi bit bật thì nhảy.
    while (queries--) {
        int planet, steps;
        cin >> planet >> steps;
        for (int level = 0; level < LEVELS; ++level) {
            if (steps & (1 << level)) {
                planet = successor[level][planet];
            }
        }
        cout << planet << '\n';
    }
    return 0;
}
