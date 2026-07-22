#include <algorithm>
#include <bitset>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

constexpr int MAX_CHILDREN = 100000;

class DisjointSetUnion {
public:
    explicit DisjointSetUnion(int size)
        : parent_(static_cast<size_t>(size)), size_(static_cast<size_t>(size), 1) {
        iota(parent_.begin(), parent_.end(), 0);
    }

    int find(int node) {
        while (parent_[static_cast<size_t>(node)] != node) {
            parent_[static_cast<size_t>(node)] =
                parent_[static_cast<size_t>(parent_[static_cast<size_t>(node)])];
            node = parent_[static_cast<size_t>(node)];
        }
        return node;
    }

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
    DisjointSetUnion dsu(children);
    for (int edge = 0; edge < wishes; ++edge) {
        int first;
        int second;
        cin >> first >> second;
        dsu.unite(first - 1, second - 1);
    }

    vector<int> frequency(static_cast<size_t>(children + 1), 0);
    for (int child = 0; child < children; ++child) {
        if (dsu.find(child) == child) {
            ++frequency[static_cast<size_t>(dsu.componentSize(child))];
        }
    }

    bitset<MAX_CHILDREN + 1> possible;
    possible[0] = true;
    for (int componentSize = 1; componentSize <= children; ++componentSize) {
        int count = frequency[static_cast<size_t>(componentSize)];
        for (int block = 1; count > 0; block *= 2) {
            const int take = min(block, count);
            possible |= possible << static_cast<size_t>(componentSize * take);
            count -= take;
        }
    }
    for (int selected = 1; selected <= children; ++selected) {
        cout << (possible[static_cast<size_t>(selected)] ? '1' : '0');
    }
    cout << '\n';
}
