#include <algorithm>
#include <bitset>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

constexpr int MAX_CHILDREN = 100000;

// DSU (union-find) để gộp các đứa trẻ buộc phải đi cùng nhau thành một khối,
// đồng thời theo dõi kích thước của mỗi khối.
class DisjointSetUnion {
public:
    explicit DisjointSetUnion(int size)
        : parent_(static_cast<size_t>(size)), size_(static_cast<size_t>(size), 1) {
        iota(parent_.begin(), parent_.end(), 0);  // ban đầu mỗi nút là gốc của chính nó
    }

    // Tìm gốc của khối chứa node, kèm nén đường đi (path halving).
    int find(int node) {
        while (parent_[static_cast<size_t>(node)] != node) {
            parent_[static_cast<size_t>(node)] =
                parent_[static_cast<size_t>(parent_[static_cast<size_t>(node)])];
            node = parent_[static_cast<size_t>(node)];
        }
        return node;
    }

    // Gộp hai khối, treo khối nhỏ vào khối lớn (union by size).
    void unite(int first, int second) {
        first = find(first);
        second = find(second);
        if (first == second) {
            return;
        }
        if (size_[static_cast<size_t>(first)] < size_[static_cast<size_t>(second)]) {
            swap(first, second);
        }
        parent_[static_cast<size_t>(second)] = first;
        size_[static_cast<size_t>(first)] += size_[static_cast<size_t>(second)];
    }

    int componentSize(int root) const {
        return size_[static_cast<size_t>(root)];
    }

private:
    vector<int> parent_;
    vector<int> size_;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int children;
    int wishes;
    cin >> children >> wishes;

    // Gộp mọi cặp yêu cầu để tạo các khối phải đi cùng địa điểm.
    DisjointSetUnion dsu(children);
    for (int edge = 0; edge < wishes; ++edge) {
        int first;
        int second;
        cin >> first >> second;
        dsu.unite(first - 1, second - 1);
    }

    // Đếm số khối ứng với từng kích thước (frequency[s] = số khối kích thước s).
    vector<int> frequency(static_cast<size_t>(children + 1), 0);
    for (int child = 0; child < children; ++child) {
        if (dsu.find(child) == child) {  // chỉ đếm tại gốc của mỗi khối
            ++frequency[static_cast<size_t>(dsu.componentSize(child))];
        }
    }

    // Bài toán trở thành tổng con: possible[i] = có thể chọn đúng i đứa trẻ.
    bitset<MAX_CHILDREN + 1> possible;
    possible[0] = true;  // chọn 0 đứa trẻ luôn đạt được
    for (int componentSize = 1; componentSize <= children; ++componentSize) {
        int count = frequency[static_cast<size_t>(componentSize)];
        // Phân rã nhị phân số lượng khối cùng kích thước để chỉ tốn O(log count)
        // phép dịch: gộp take khối thành trọng lượng componentSize * take.
        for (int block = 1; count > 0; block *= 2) {
            const int take = min(block, count);
            possible |= possible << static_cast<size_t>(componentSize * take);
            count -= take;
        }
    }

    // In xâu nhị phân: bit i cho biết có thể chọn đúng i đứa trẻ hay không.
    for (int selected = 1; selected <= children; ++selected) {
        cout << (possible[static_cast<size_t>(selected)] ? '1' : '0');
    }
    cout << '\n';
}
