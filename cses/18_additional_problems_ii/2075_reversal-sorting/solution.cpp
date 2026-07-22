#include <algorithm>
#include <cstdint>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

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

void applyReverse(Node* node) {
    if (node != nullptr) {
        swap(node->left, node->right);
        node->reversed = !node->reversed;
    }
}

void push(Node* node) {
    if (node != nullptr && node->reversed) {
        applyReverse(node->left);
        applyReverse(node->right);
        node->reversed = false;
    }
}

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

    vector<pair<int, int>> operations;
    for (int value = 1; value <= n; ++value) {
        const int position = positionOf(byValue[value]);
        if (position == value) {
            continue;
        }
        operations.push_back({value, position});
        Node* prefix = nullptr;
        Node* suffix = nullptr;
        Node* middle = nullptr;
        split(root, value - 1, prefix, suffix);
        split(suffix, position - value + 1, middle, suffix);
        applyReverse(middle);
        root = merge(prefix, merge(middle, suffix));
    }

    cout << operations.size() << '\n';
    for (const auto [left, right] : operations) {
        cout << left << ' ' << right << '\n';
    }
}
