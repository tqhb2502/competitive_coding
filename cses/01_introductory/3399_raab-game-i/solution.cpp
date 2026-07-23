#include <iostream>
#include <vector>

using namespace std;

// In một dãy bài (hoán vị) trên một dòng, các số cách nhau bởi dấu cách.
void printPermutation(const vector<int>& permutation) {
    for (size_t i = 0; i < permutation.size(); ++i) {
        if (i > 0) {
            cout << ' ';
        }
        cout << permutation[i];
    }
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tests;
    cin >> tests;
    while (tests--) {
        int n, a, b;
        cin >> n >> a >> b;

        // Điều kiện khả thi:
        //  - a + b <= n (phần còn lại là các ván hòa);
        //  - a == 0 khi và chỉ khi b == 0 (không thể có excedance mà thiếu deficiency).
        if (a + b > n || ((a == 0) != (b == 0))) {
            cout << "NO\n";
            continue;
        }

        // m lá bài đầu tham gia phân thắng thua; n - m lá còn lại đều hòa.
        const int moved = a + b;
        vector<int> firstPlayer;
        vector<int> secondPlayer;
        firstPlayer.reserve(n);
        secondPlayer.reserve(n);

        // Người 1: dịch vòng tròn theo b trên m lá đầu -> [b+1..m] + [1..b], rồi [m+1..n].
        for (int card = b + 1; card <= moved; ++card) {
            firstPlayer.push_back(card);
        }
        for (int card = 1; card <= b; ++card) {
            firstPlayer.push_back(card);
        }
        for (int card = moved + 1; card <= n; ++card) {
            firstPlayer.push_back(card);
        }
        // Người 2: đánh theo thứ tự 1..n.
        for (int card = 1; card <= n; ++card) {
            secondPlayer.push_back(card);
        }

        cout << "YES\n";
        printPermutation(firstPlayer);
        printPermutation(secondPlayer);
    }

    return 0;
}
