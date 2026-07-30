#include <iostream>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    // Giá trị mex tại ô (row, col) theo 0-indexed chính là row XOR col.
    // In từng hàng: các giá trị cách nhau bởi dấu cách, mỗi hàng một dòng.
    for (int row = 0; row < n; ++row) {
        for (int col = 0; col < n; ++col) {
            if (col > 0) {
                cout << ' ';
            }
            cout << (row ^ col);
        }
        cout << '\n';
    }

    return 0;
}
