#include <iostream>
#include <string>

using namespace std;

int main() {
    // Fast I/O: tắt đồng bộ với stdio và bỏ ràng buộc cin/cout
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    const int total = 1 << n;   // tổng số xâu cần in là 2^n
    for (int i = 0; i < total; ++i) {
        // Công thức Gray code chuẩn: gray(i) = i XOR (i >> 1)
        const int gray = i ^ (i >> 1);

        // Chuyển gray thành xâu nhị phân đúng n bit (mặc định toàn số 0)
        string code(n, '0');
        for (int bit = 0; bit < n; ++bit) {
            // Nếu bit thứ 'bit' bật thì đặt '1' vào vị trí tương ứng
            if (gray & (1 << bit)) {
                code[n - 1 - bit] = '1';
            }
        }
        cout << code << '\n';
    }

    return 0;
}
