#include <iostream>
#include <utility>
#include <vector>

using namespace std;

// Đệ quy chuẩn: chuyển "disks" đĩa trên cùng từ cọc "source" sang cọc
// "destination", dùng cọc "auxiliary" làm trung gian; ghi lại từng bước vào moves.
void hanoi(int disks, int source, int destination, int auxiliary,
           vector<pair<int, int>>& moves) {
    if (disks == 0) {
        return;
    }

    // 1) Dọn disks-1 đĩa nhỏ hơn sang cọc phụ (cọc đích tạm làm trung gian).
    hanoi(disks - 1, source, auxiliary, destination, moves);
    // 2) Chuyển đĩa lớn nhất của nhóm từ nguồn sang đích -> ghi 1 bước.
    moves.emplace_back(source, destination);
    // 3) Chuyển disks-1 đĩa nhỏ từ cọc phụ về cọc đích (cọc nguồn làm trung gian).
    hanoi(disks - 1, auxiliary, destination, source, moves);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int disks;
    cin >> disks;

    // Tổng số bước tối thiểu là 2^disks - 1, đặt trước dung lượng cho moves.
    vector<pair<int, int>> moves;
    moves.reserve((1U << disks) - 1);
    hanoi(disks, 1, 3, 2, moves);

    // In số bước, rồi từng bước "a b" (chuyển đĩa trên cùng từ cọc a sang cọc b).
    cout << moves.size() << '\n';
    for (const auto& [source, destination] : moves) {
        cout << source << ' ' << destination << '\n';
    }
    return 0;
}
