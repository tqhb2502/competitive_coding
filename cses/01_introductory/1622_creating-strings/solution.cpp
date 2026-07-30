#include <algorithm>
#include <iostream>
#include <set>
#include <string>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    // Sắp xếp các ký tự tăng dần để next_permutation duyệt từ hoán vị nhỏ nhất.
    sort(s.begin(), s.end());

    // next_permutation duyệt lần lượt các hoán vị phân biệt theo thứ tự từ điển tăng dần; set chỉ để lưu kết quả (và giữ thứ tự từ điển).
    set<string> distinct_permutations;
    do {
        distinct_permutations.insert(s);
    } while (next_permutation(s.begin(), s.end()));

    // In số lượng hoán vị phân biệt, rồi in từng xâu theo thứ tự từ điển.
    cout << distinct_permutations.size() << '\n';
    for (const string& permutation : distinct_permutations) {
        cout << permutation << '\n';
    }
    return 0;
}
