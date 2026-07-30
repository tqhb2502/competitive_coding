#include <algorithm>
#include <array>
#include <iostream>
#include <set>
#include <tuple>
#include <vector>

using namespace std;

// Leftist heap dạng mảng: giữ các out (thời điểm rời stack) của mỗi lớp parity,
// hỗ trợ merge và pop lấy phần tử nhỏ nhất trong O(log n).
class LeftistHeap {
public:
    explicit LeftistHeap(int size)
        : key_(size + 1, 0), left_(size + 1, 0), right_(size + 1, 0),
          rank_(size + 1, 1) {
        rank_[0] = 0;
    }

    // Khởi tạo một nút lá mang khóa cho trước.
    void initializeNode(int node, int key) {
        key_[node] = key;
        left_[node] = 0;
        right_[node] = 0;
        rank_[node] = 1;
    }

    // Gộp hai heap, giữ tính chất leftist (nhánh phải luôn ngắn hơn).
    int merge(int first, int second) {
        if (first == 0) {
            return second;
        }
        if (second == 0) {
            return first;
        }
        // Chọn khóa nhỏ hơn làm gốc; dùng chỉ số nút để phá hòa ổn định.
        if (pair<int, int>{key_[second], second} <
            pair<int, int>{key_[first], first}) {
            swap(first, second);
        }
        right_[first] = merge(right_[first], second);
        if (rank_[left_[first]] < rank_[right_[first]]) {
            swap(left_[first], right_[first]);
        }
        rank_[first] = rank_[right_[first]] + 1;
        return first;
    }

    // Bỏ gốc (khóa nhỏ nhất) rồi gộp hai nhánh con lại.
    int pop(int root) {
        return merge(left_[root], right_[root]);
    }

    int key(int root) const {
        return key_[root];
    }

private:
    vector<int> key_;
    vector<int> left_;
    vector<int> right_;
    vector<int> rank_;
};

// DSU có parity: gộp các đỉnh với ràng buộc cùng/khác màu, đồng thời mỗi thành
// phần lưu hai leftist heap ứng với hai lớp parity (hai màu tương đối).
class ParityComponents {
public:
    ParityComponents(int size, LeftistHeap& heap)
        : parent_(size + 1), componentSize_(size + 1, 1),
          parityToParent_(size + 1, 0), heaps_(size + 1, {0, 0}),
          inGlobal_(size + 1, {false, false}), heap_(heap) {
        for (int vertex = 0; vertex <= size; ++vertex) {
            parent_[vertex] = vertex;
        }
    }

    // Tìm gốc và parity (màu tương đối so với gốc) của một đỉnh, có nén đường.
    pair<int, int> find(int vertex) {
        if (parent_[vertex] == vertex) {
            return {vertex, 0};
        }
        const auto [root, parentParity] = find(parent_[vertex]);
        parityToParent_[vertex] ^= parentParity;
        parent_[vertex] = root;
        return {root, parityToParent_[vertex]};
    }

    // Gộp hai đỉnh với parity yêu cầu; trả về false nếu mâu thuẫn màu.
    bool unite(int first, int second, int requiredParity) {
        auto [firstRoot, firstParity] = find(first);
        auto [secondRoot, secondParity] = find(second);
        if (firstRoot == secondRoot) {
            // Đã cùng thành phần: chỉ cần kiểm tra parity có khớp không.
            return (firstParity ^ secondParity) == requiredParity;
        }

        // Gộp theo kích thước (union by size) để giữ cây thấp.
        if (componentSize_[firstRoot] < componentSize_[secondRoot]) {
            swap(firstRoot, secondRoot);
        }
        const int rootParity = firstParity ^ secondParity ^ requiredParity;
        parent_[secondRoot] = firstRoot;
        parityToParent_[secondRoot] = rootParity;
        componentSize_[firstRoot] += componentSize_[secondRoot];

        // Gộp các heap của thành phần bị hợp vào, dịch parity cho phù hợp.
        for (int parity = 0; parity < 2; ++parity) {
            const int destinationParity = parity ^ rootParity;
            heaps_[firstRoot][destinationParity] = heap_.merge(
                heaps_[firstRoot][destinationParity],
                heaps_[secondRoot][parity]);
        }
        return true;
    }

    array<int, 2>& heaps(int root) {
        return heaps_[root];
    }

    array<bool, 2>& globalFlags(int root) {
        return inGlobal_[root];
    }

private:
    vector<int> parent_;
    vector<int> componentSize_;
    vector<int> parityToParent_;
    vector<array<int, 2>> heaps_;
    vector<array<bool, 2>> inGlobal_;
    LeftistHeap& heap_;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int size = 0;
    cin >> size;
    vector<int> permutation(size);
    for (int& value : permutation) {
        cin >> value;
    }

    // Mô phỏng lịch tham lam để tính khoảng sống [in, out] của mỗi giá trị:
    // đẩy vào khi cần, lấy ra ngay khi giá trị nhỏ nhất đang chờ đã có mặt.
    vector<int> enterTime(size + 1, 0);
    vector<int> leaveTime(size + 1, 0);
    vector<char> entered(size + 1, false);
    int inputIndex = 0;
    int nextOutput = 1;
    int time = 0;
    while (nextOutput <= size) {
        ++time;
        if (entered[nextOutput]) {
            leaveTime[nextOutput] = time;
            ++nextOutput;
        } else {
            const int value = permutation[inputIndex];
            ++inputIndex;
            entered[value] = true;
            enterTime[value] = time;
        }
    }

    LeftistHeap heap(size);
    ParityComponents components(size, heap);
    // Set toàn cục: out nhỏ nhất của mỗi (thành phần, parity) đang hoạt động.
    set<tuple<int, int, int>> globalMinimum;

    // Gỡ mọi mục của một thành phần khỏi set toàn cục.
    const auto removeRoot = [&](int root) {
        auto& roots = components.heaps(root);
        auto& flags = components.globalFlags(root);
        for (int parity = 0; parity < 2; ++parity) {
            if (flags[parity]) {
                globalMinimum.erase(
                    {heap.key(roots[parity]), root, parity});
                flags[parity] = false;
            }
        }
    };

    // Đưa lại thành phần vào set toàn cục, có thể tạm ẩn một parity chỉ định.
    const auto insertRoot = [&](int root, int suppressedParity) {
        auto& roots = components.heaps(root);
        auto& flags = components.globalFlags(root);
        for (int parity = 0; parity < 2; ++parity) {
            if (parity != suppressedParity && roots[parity] != 0) {
                globalMinimum.emplace(heap.key(roots[parity]), root, parity);
                flags[parity] = true;
            }
        }
    };

    bool possible = true;
    // Quét các khoảng theo thứ tự đầu vào (cũng là thứ tự đầu trái tăng dần).
    for (const int current : permutation) {
        const int leftEndpoint = enterTime[current];
        const int rightEndpoint = leaveTime[current];

        // Xóa lười các out đã kết thúc trước đầu trái khoảng hiện tại.
        while (!globalMinimum.empty() &&
               get<0>(*globalMinimum.begin()) <= leftEndpoint) {
            const int root = get<1>(*globalMinimum.begin());
            removeRoot(root);
            auto& roots = components.heaps(root);
            for (int parity = 0; parity < 2; ++parity) {
                while (roots[parity] != 0 &&
                       heap.key(roots[parity]) <= leftEndpoint) {
                    roots[parity] = heap.pop(roots[parity]);
                }
            }
            insertRoot(root, -1);
        }

        // Mọi khoảng còn out nhỏ hơn out mới đều xung đột: buộc gộp parity 1
        // (ngược màu) với khoảng hiện tại; phát hiện mâu thuẫn nếu có.
        while (!globalMinimum.empty() &&
               get<0>(*globalMinimum.begin()) < rightEndpoint) {
            const auto [minimumRight, otherRoot, otherParity] =
                *globalMinimum.begin();
            static_cast<void>(minimumRight);
            const auto [currentRoot, currentParity] = components.find(current);

            if (otherRoot == currentRoot) {
                // Cùng thành phần mà lại cùng parity thì không thể ngược màu.
                if (otherParity == currentParity) {
                    possible = false;
                }
                break;
            }

            const int representative =
                components.heaps(otherRoot)[otherParity];
            removeRoot(currentRoot);
            removeRoot(otherRoot);
            if (!components.unite(current, representative, 1)) {
                possible = false;
                break;
            }
            // Gộp xong: lớp đã biết ngược màu đỉnh mới tạm ẩn khỏi set.
            const auto [newRoot, newCurrentParity] = components.find(current);
            insertRoot(newRoot, newCurrentParity ^ 1);
        }
        if (!possible) {
            break;
        }

        // Thêm out của khoảng hiện tại vào heap của lớp parity tương ứng.
        const auto [root, currentParity] = components.find(current);
        removeRoot(root);
        heap.initializeNode(current, rightEndpoint);
        auto& roots = components.heaps(root);
        roots[currentParity] = heap.merge(roots[currentParity], current);
        insertRoot(root, -1);
    }

    // Lấy parity cuối làm màu (số stack) rồi mô phỏng lại để xác nhận đáp án.
    vector<int> color(size + 1, 0);
    if (possible) {
        for (int value = 1; value <= size; ++value) {
            color[value] = components.find(value).second;
        }

        array<vector<int>, 2> stacks;
        nextOutput = 1;
        // Lấy liên tục các đỉnh stack đúng bằng giá trị đang cần xuất.
        const auto drain = [&]() {
            bool changed = true;
            while (changed) {
                changed = false;
                for (int stackIndex = 0; stackIndex < 2; ++stackIndex) {
                    if (!stacks[stackIndex].empty() &&
                        stacks[stackIndex].back() == nextOutput) {
                        stacks[stackIndex].pop_back();
                        ++nextOutput;
                        changed = true;
                        break;
                    }
                }
            }
        };

        for (const int value : permutation) {
            drain();
            stacks[color[value]].push_back(value);
            drain();
        }
        drain();
        // Chỉ hợp lệ nếu đã xuất hết đúng dãy 1..n.
        possible = nextOutput == size + 1;
    }

    if (!possible) {
        cout << "IMPOSSIBLE\n";
        return 0;
    }
    // In số hiệu stack (1 hoặc 2) của từng phần tử theo thứ tự đầu vào.
    for (int index = 0; index < size; ++index) {
        cout << color[permutation[index]] + 1
             << (index + 1 == size ? '\n' : ' ');
    }
}
