#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

// Fenwick tree dùng để đếm số nghịch thế
class FenwickTree {
public:
    explicit FenwickTree(int size) : tree(size + 1, 0) {}

    void add(int index, int value) {
        const int size = static_cast<int>(tree.size());
        for (int i = index; i < size; i += i & -i) {
            tree[i] += value;
        }
    }

    int prefixSum(int index) const {
        int result = 0;
        for (int i = index; i > 0; i -= i & -i) {
            result += tree[i];
        }
        return result;
    }

private:
    vector<int> tree;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> permutation(n);
    for (int &value : permutation) {
        cin >> value;
    }

    // Thao tác 1 - đổi kề: đáp án bằng số nghịch thế.
    // Với mỗi phần tử, số nghịch thế mới là số giá trị đã gặp mà lớn hơn nó.
    FenwickTree seen(n);
    long long inversions = 0;
    for (int index = 0; index < n; ++index) {
        inversions += index - seen.prefixSum(permutation[index]);
        seen.add(permutation[index], 1);
    }

    // Thao tác 2 - đổi bất kỳ: đáp án bằng n trừ số chu trình của hoán vị.
    vector<bool> visited(n + 1, false);
    int cycles = 0;
    for (int start = 1; start <= n; ++start) {
        if (visited[start]) {
            continue;
        }
        ++cycles;
        int current = start;
        while (!visited[current]) {
            visited[current] = true;
            current = permutation[current - 1];
        }
    }
    const int arbitrarySwaps = n - cycles;

    // Thao tác 3 - chuyển tới vị trí bất kỳ: đáp án bằng n trừ độ dài LIS.
    // Mảng tails lưu phần tử cuối nhỏ nhất của dãy con tăng theo từng độ dài.
    vector<int> tails;
    tails.reserve(n);
    for (int value : permutation) {
        const auto place = lower_bound(tails.begin(), tails.end(), value);
        if (place == tails.end()) {
            tails.push_back(value);
        } else {
            *place = value;
        }
    }
    const int arbitraryMoves = n - static_cast<int>(tails.size());

    // Thao tác 4 - chuyển ra đầu: quét từ phải sang trái, lần lượt tìm n, n-1, ...
    // để xác định hậu tố giá trị liên tiếp giữ nguyên; số giá trị còn lại là đáp án.
    int expected = n;
    for (int index = n - 1; index >= 0; --index) {
        if (permutation[index] == expected) {
            --expected;
        }
    }
    const int movesToFront = expected;

    cout << inversions << ' ' << arbitrarySwaps << ' ' << arbitraryMoves << ' '
         << movesToFront << '\n';
}
