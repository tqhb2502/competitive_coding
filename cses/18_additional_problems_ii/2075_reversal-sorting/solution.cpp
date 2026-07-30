#include <algorithm>
#include <cstdint>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

// Node của implicit treap: giữ giá trị, kích thước cây con, độ ưu tiên,
// cờ lazy reverse và các con trỏ trái/phải/cha.
struct Node {
    int value = 0;
    int subtreeSize = 1;
    uint32_t priority = 0;
    bool reversed = false;
    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;
};

int sizeOf(const Node* node) {
    return node == nullptr ? 0 : node->subtreeSize;
}

// Cập nhật kích thước cây con và gắn lại con trỏ cha cho hai con.
void update(Node* node) {
    if (node == nullptr) {
        return;
    }
    node->subtreeSize = 1 + sizeOf(node->left) + sizeOf(node->right);
    if (node->left != nullptr) {
        node->left->parent = node;
    }
    if (node->right != nullptr) {
        node->right->parent = node;
    }
}

// Đánh dấu đảo một cây con: đổi chỗ hai con và lật cờ lazy reverse.
void applyReverse(Node* node) {
    if (node != nullptr) {
        swap(node->left, node->right);
        node->reversed = !node->reversed;
    }
}

// Đẩy cờ lazy reverse xuống hai con trước khi đi tiếp.
void push(Node* node) {
    if (node != nullptr && node->reversed) {
        applyReverse(node->left);
        applyReverse(node->right);
        node->reversed = false;
    }
}

// Gộp hai cây (mọi khóa của left đứng trước right) theo độ ưu tiên treap.
Node* merge(Node* left, Node* right) {
    if (left == nullptr) {
        if (right != nullptr) {
            right->parent = nullptr;
        }
        return right;
    }
    if (right == nullptr) {
        left->parent = nullptr;
        return left;
    }
    push(left);
    push(right);
    if (left->priority > right->priority) {
        left->right = merge(left->right, right);
        update(left);
        left->parent = nullptr;
        return left;
    }
    right->left = merge(left, right->left);
    update(right);
    right->parent = nullptr;
    return right;
}

// Tách cây thành hai phần: left gồm leftSize phần tử đầu, right là phần còn lại.
void split(Node* root, int leftSize, Node*& left, Node*& right) {
    if (root == nullptr) {
        left = nullptr;
        right = nullptr;
        return;
    }
    push(root);
    if (sizeOf(root->left) >= leftSize) {
        split(root->left, leftSize, left, root->left);
        update(root);
        right = root;
        right->parent = nullptr;
    } else {
        split(root->right, leftSize - sizeOf(root->left) - 1, root->right,
              right);
        update(root);
        left = root;
        left->parent = nullptr;
    }
}

// Tính vị trí (1-based) của node: trước tiên đẩy lazy từ gốc xuống, rồi cộng
// kích thước cây con trái với các nhánh trái gặp trên đường đi cha lên gốc.
int positionOf(Node* node) {
    vector<Node*> ancestors;
    for (Node* current = node; current != nullptr; current = current->parent) {
        ancestors.push_back(current);
    }
    reverse(ancestors.begin(), ancestors.end());
    for (Node* ancestor : ancestors) {
        push(ancestor);
    }

    int position = sizeOf(node->left) + 1;
    for (Node* current = node; current->parent != nullptr;
         current = current->parent) {
        if (current == current->parent->right) {
            position += sizeOf(current->parent->left) + 1;
        }
    }
    return position;
}

// Bộ sinh số giả ngẫu nhiên xorshift cho độ ưu tiên treap.
uint32_t nextRandom() {
    static uint32_t state = 712367821U;
    state ^= state << 13U;
    state ^= state >> 17U;
    state ^= state << 5U;
    return state;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    // Đọc hoán vị, tạo node cho từng phần tử và gộp dần vào cây.
    // byValue[v] trỏ tới node chứa giá trị v để tra vị trí về sau.
    vector<Node> nodes(n);
    vector<Node*> byValue(n + 1, nullptr);
    Node* root = nullptr;
    for (int index = 0; index < n; ++index) {
        int value;
        cin >> value;
        nodes[index].value = value;
        nodes[index].priority = nextRandom();
        byValue[value] = &nodes[index];
        root = merge(root, &nodes[index]);
    }

    // Đặt lần lượt giá trị 1..n về đúng chỗ; nếu đang lệch thì đảo đoạn
    // [value, position] để đưa value về vị trí value.
    vector<pair<int, int>> operations;
    for (int value = 1; value <= n; ++value) {
        const int position = positionOf(byValue[value]);
        if (position == value) {
            continue;
        }
        operations.push_back({value, position});
        // Tách lấy đoạn giữa [value, position], đảo nó rồi gộp lại.
        Node* prefix = nullptr;
        Node* suffix = nullptr;
        Node* middle = nullptr;
        split(root, value - 1, prefix, suffix);
        split(suffix, position - value + 1, middle, suffix);
        applyReverse(middle);
        root = merge(prefix, merge(middle, suffix));
    }

    // In số phép đảo rồi tới từng cặp chỉ số đoạn đã đảo.
    cout << operations.size() << '\n';
    for (const auto [left, right] : operations) {
        cout << left << ' ' << right << '\n';
    }
}
