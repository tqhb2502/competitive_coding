#include <iostream>
#include <vector>

using namespace std;

// In một tập: dòng đầu là số phần tử, dòng sau là các phần tử cách nhau bởi dấu cách.
void print_set(const vector<int>& values) {
    cout << values.size() << '\n';
    for (size_t i = 0; i < values.size(); ++i) {
        if (i > 0) {
            cout << ' ';
        }
        cout << values[i];
    }
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    // Tổng 1..n = n*(n+1)/2 chỉ chẵn khi n % 4 == 0 hoặc n % 4 == 3; ngoài ra không chia được.
    if (n % 4 != 0 && n % 4 != 3) {
        cout << "NO\n";
        return 0;
    }

    // Mỗi tập cần đạt tổng target = S/2 = n*(n+1)/4.
    const long long target = 1LL * n * (n + 1) / 4;
    long long remaining = target;
    vector<int> first_set;
    vector<int> second_set;

    // Tham lam: duyệt số lớn nhất về nhỏ nhất, lấy i vào tập 1 khi còn "vừa" (i <= remaining),
    // phần còn lại dồn sang tập 2 nên tổng tập 2 cũng bằng target.
    for (int value = n; value >= 1; --value) {
        if (value <= remaining) {
            first_set.push_back(value);
            remaining -= value;
        } else {
            second_set.push_back(value);
        }
    }

    cout << "YES\n";
    print_set(first_set);
    print_set(second_set);
    return 0;
}
