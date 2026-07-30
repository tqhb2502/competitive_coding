#pragma once
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace std;
using namespace __gnu_pbds;
// Ordered Set (PBDS) — set thống kê thứ tự: find_by_order (nhỏ thứ k) + order_of_key (đếm nhỏ hơn x), O(log n).
// Khi dùng: cần "phần tử nhỏ thứ k" và "số phần tử nhỏ hơn x" trên tập ĐỘNG; ngắn và nhanh hơn tự viết treap.
// ĐPT: insert/erase/find_by_order/order_of_key O(log n); CHỈ chạy trên g++ (đuôi mở rộng GCC, không portable).
// Dùng: ordered_set<int> s; s.insert(x); *s.find_by_order(k); // nhỏ thứ k (0-based); s.order_of_key(x); // số phần tử < x
// Bẫy: set PHÂN BIỆT (không trùng); muốn multiset -> dùng ordered_set<pair<T,int>> gắn chỉ số phụ; chỉ dùng với GCC.
// CSES: 1144 1749 2163
template <class T, class Compare = less<T>>
using ordered_set =
    tree<T, null_type, Compare, rb_tree_tag, tree_order_statistics_node_update>;

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ ordered-set.hpp -o demo && ./demo
int main() {
    ordered_set<int> s;
    for (int x : {5, 1, 9, 3, 7}) s.insert(x);   // {1,3,5,7,9}
    printf("nhỏ thứ 2 (0-based)=%d  số phần tử < 6=%d\n",
           *s.find_by_order(1), (int)s.order_of_key(6));   // 3   3
    return 0;
}
#endif
