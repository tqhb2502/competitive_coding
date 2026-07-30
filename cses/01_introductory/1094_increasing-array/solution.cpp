#include <iostream>

using namespace std;

int main() {
    // Tăng tốc đọc/ghi dữ liệu
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    // moves: tổng số bước cần thực hiện
    // previous: giá trị phần tử liền trước sau khi đã chỉnh sửa
    // Dùng long long vì kết quả tối đa có thể ~ 2*10^14, vượt quá int 32-bit
    long long moves = 0;
    long long previous = 0;

    // Duyệt mảng từ trái sang phải theo tư tưởng greedy (tham lam)
    for (int i = 0; i < n; ++i) {
        long long value;
        cin >> value;
        if (value < previous) {
            // Phần tử nhỏ hơn prev: phải tăng nó lên bằng prev,
            // tốn (previous - value) bước; prev giữ nguyên
            moves += previous - value;
        } else {
            // Phần tử đã >= prev: không cần chỉnh, cập nhật prev = value
            previous = value;
        }
    }

    cout << moves << '\n';
    return 0;
}
