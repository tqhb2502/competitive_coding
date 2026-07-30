#pragma once
#include <bits/stdc++.h>
using namespace std;
// Leftist Heap — priority queue meldable: gộp hai heap phá hủy trong O(log n), nhiều root chung một pool.
// Khi dùng: DSU cần gộp heap của component; divide-and-conquer/graph cần meld nhanh hơn std::priority_queue.
// ĐPT: make/top O(1); meld/push/pop O(log n); bộ nhớ O(tổng số lần push). Root là handle int, -1 = rỗng.
// Dùng: LeftistHeap<int> pool; int a=-1,b=-1; a=pool.push(a,x); a=pool.meld(a,b); a=pool.pop(a);
// Bẫy: meld/push/pop trả ROOT MỚI; không dùng lại hai root cũ sau meld; node đã pop không thu hồi.
// CSES: 2402

template<class T, class Compare = less<T>>
struct LeftistHeap {
    struct Node {
        T key;
        int left = -1, right = -1;
        int rank = 1, subtreeSize = 1;
    };
    vector<Node> nodes;
    Compare higherPriority;

    // Ý nghĩa: khởi tạo pool trống, có thể reserve số node để tránh realloc.
    // Tham số: reserveNodes = số lần push dự kiến; compare ưu tiên key nào lên top (mặc định min-heap).
    explicit LeftistHeap(int reserveNodes = 0, Compare compare = Compare())
        : higherPriority(std::move(compare)) {
        nodes.reserve(reserveNodes);
    }
    // Ý nghĩa: tạo heap một phần tử.
    // Tham số: key = khóa của node mới.
    // Trả về: root handle mới.
    int make(const T& key) {
        nodes.push_back(Node{key});
        return (int)nodes.size() - 1;
    }
    // Ý nghĩa: rank đường phải của root, bằng 0 cho heap rỗng.
    int rankOf(int root) const { return root == -1 ? 0 : nodes[root].rank; }
    // Ý nghĩa: số node sống trong cây root, bằng 0 cho heap rỗng.
    int size(int root) const { return root == -1 ? 0 : nodes[root].subtreeSize; }
    // Ý nghĩa: kiểm tra root có đại diện heap rỗng hay không.
    bool empty(int root) const { return root == -1; }
    // Ý nghĩa: gộp phá hủy hai heap và giữ bất biến leftist; root cũ không còn độc lập.
    // Tham số: first,second = hai root handle (được phép -1).
    // Trả về: root của heap hợp.
    int meld(int first, int second) {
        if (first == -1) return second;
        if (second == -1) return first;
        if (higherPriority(nodes[second].key, nodes[first].key)) swap(first, second);
        nodes[first].right = meld(nodes[first].right, second);
        if (rankOf(nodes[first].left) < rankOf(nodes[first].right))
            swap(nodes[first].left, nodes[first].right);
        nodes[first].rank = rankOf(nodes[first].right) + 1;
        nodes[first].subtreeSize = 1 + size(nodes[first].left) + size(nodes[first].right);
        return first;
    }
    // Ý nghĩa: thêm key vào heap root.
    // Tham số: root = heap hiện tại; key = khóa mới.
    // Trả về: root mới sau meld với heap một phần tử.
    int push(int root, const T& key) { return meld(root, make(key)); }
    // Ý nghĩa: đọc khóa ưu tiên cao nhất.
    // Tham số: root = root khác -1.
    // Trả về: tham chiếu const tới key ở root.
    const T& top(int root) const {
        assert(root != -1);
        return nodes[root].key;
    }
    // Ý nghĩa: bỏ node top bằng cách meld hai cây con.
    // Tham số: root = root khác -1.
    // Trả về: root mới sau khi pop.
    int pop(int root) {
        assert(root != -1);
        return meld(nodes[root].left, nodes[root].right);
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ leftist-heap.hpp -o demo && ./demo
int main() {
    LeftistHeap<int> heap;
    int a = -1, b = -1;
    for (int x : {7, 2, 9}) a = heap.push(a, x);
    for (int x : {5, 1}) b = heap.push(b, x);
    a = heap.meld(a, b);
    printf("meld size=%d:", heap.size(a));
    while (!heap.empty(a)) {
        printf(" %d", heap.top(a));
        a = heap.pop(a);
    }
    printf("\n");
    return 0;
}
#endif
