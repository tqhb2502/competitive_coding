#pragma once
#include <bits/stdc++.h>
// Debug in ra STDERR — bật bằng cờ -DCP_DEBUG, KHÔNG bật thì mọi dbg(...) biến mất hoàn toàn.
// Khi dùng: cần xem giá trị biến/vector/pair/map lúc gỡ lỗi mà không làm bẩn output nộp bài.
// ĐPT: chỉ tồn tại khi biên dịch với -DCP_DEBUG; khi nộp (không cờ) macro rỗng nên không tốn gì.
// Dùng: #include "cp_library/templates/debug.hpp" rồi dbg(x, y, v);  // g++ -DCP_DEBUG ... để bật
// Bẫy: in ra cerr (không phải cout) nên không lẫn vào đáp án; KHÔNG đặt tính toán có tác dụng phụ vào dbg().
// CSES: —
#ifdef CP_DEBUG
namespace cpdbg {
// Ý nghĩa: in một giá trị ra cerr; tự nhận biết số, xâu, và mọi container duyệt được.
// Tham số: v = giá trị cần in.
template <class T> void out(const T& v);
// Ý nghĩa: in một cặp (pair) ra cerr dưới dạng (first, second).
// Tham số: p = cặp cần in.
template <class A, class B> void out(const std::pair<A, B>& p);
template <class T> void out(const T& v) {
    if constexpr (std::is_convertible_v<T, std::string_view>) std::cerr << '"' << v << '"';
    else if constexpr (std::is_arithmetic_v<T>) std::cerr << v;
    else {
        std::cerr << '{';
        bool first = true;
        for (const auto& e : v) { if (!first) std::cerr << ", "; first = false; out(e); }
        std::cerr << '}';
    }
}
template <class A, class B> void out(const std::pair<A, B>& p) {
    std::cerr << '(';
    out(p.first);
    std::cerr << ", ";
    out(p.second);
    std::cerr << ')';
}
// Ý nghĩa: in lần lượt "tên = giá trị" cho danh sách biến mà macro dbg truyền vào.
// Tham số: names = chuỗi tên biến do #__VA_ARGS__ sinh; v, rest = các giá trị tương ứng.
inline void all(const char*) { std::cerr << '\n'; }
template <class T, class... R> void all(const char* names, const T& v, const R&... rest) {
    const char* comma = std::strchr(names, ',');
    while (*names == ' ') names++;
    std::cerr.write(names, comma ? comma - names : (long long)std::strlen(names));
    std::cerr << " = ";
    out(v);
    if constexpr (sizeof...(rest) > 0) { std::cerr << " | "; all(comma + 1, rest...); }
    else std::cerr << '\n';
}
}  // namespace cpdbg
#define dbg(...) (std::cerr << "[dbg L" << __LINE__ << "] ", cpdbg::all(#__VA_ARGS__, __VA_ARGS__))
#else
#define dbg(...) ((void)0)
#endif

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -DCP_DEBUG -x c++ debug.hpp -o demo && ./demo
int main() {
    int x = 42;
    std::vector<int> v = {1, 2, 3};
    std::vector<std::pair<int, int>> vp = {{1, 2}, {3, 4}};
    dbg(x, v, vp);            // in ra stderr: [dbg L..] x = 42 | v = {1, 2, 3} | vp = {(1, 2), (3, 4)}
    (void)x; (void)v; (void)vp;   // khi TẮT -DCP_DEBUG thì dbg() rỗng, tránh cảnh báo biến không dùng
    printf("stdout sạch\n");  // stdout không bị lẫn thông tin debug
    return 0;
}
#endif
