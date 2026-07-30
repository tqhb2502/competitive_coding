#pragma once
#include <bits/stdc++.h>
using namespace std;
// Backtracking — khung chọn/hoàn tác theo độ sâu, hỗ trợ cắt tỉa và sắp thứ tự ứng viên.
// Khi dùng: n nhỏ; cần liệt kê/đếm cấu hình thỏa ràng buộc, Hamilton/queens/grid path với undo state.
// ĐPT: O(số node trạng thái thực sự duyệt * chi phí sinh ứng viên); bộ nhớ O(depth + trạng thái).
// Dùng: backtrackCount(depth, choices, take, undo, accept); take trả false để prune ngay nhánh.
// Bẫy: undo phải đảo CHÍNH XÁC mọi thay đổi của take; depth đệ quy quá lớn sẽ tràn stack; xếp ứng viên tốt trước để prune.
// CSES: 1622 1624 1625 1689
// Ý nghĩa: đếm nghiệm bằng DFS quay lui tổng quát; mỗi take thành công được ghép đúng một lần undo.
// Tham số: targetDepth = số tầng; choices(depth) trả container ứng viên; take(depth,c) áp dụng và trả có đi tiếp không; undo hoàn tác take thành công; accept kiểm tra lá.
// Trả về: số lá ở depth=targetDepth mà accept() trả true.
template <class Choices, class Take, class Undo, class Accept>
long long backtrackCount(int targetDepth, Choices choices, Take take, Undo undo, Accept accept) {
    function<long long(int)> dfs = [&](int depth) -> long long {
        if (depth == targetDepth) return accept() ? 1LL : 0LL;
        long long ways = 0;
        for (const auto& candidate : choices(depth)) {
            bool ok = take(depth, candidate);
            if (ok) {
                ways += dfs(depth + 1);
                undo(depth, candidate);
            }
        }
        return ways;
    };
    return dfs(0);
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ backtracking.hpp -o demo && ./demo
int main() {
    const int n = 8;
    array<bool, 8> col{};
    array<bool, 15> diagDown{}, diagUp{};
    auto choices = [&](int) {
        array<int, 8> all{};
        iota(all.begin(), all.end(), 0);
        return all;
    };
    auto take = [&](int row, int c) {
        if (col[c] || diagDown[row - c + 7] || diagUp[row + c]) return false;
        col[c] = diagDown[row - c + 7] = diagUp[row + c] = true;
        return true;
    };
    auto undo = [&](int row, int c) {
        col[c] = diagDown[row - c + 7] = diagUp[row + c] = false;
    };
    long long ways = backtrackCount(n, choices, take, undo, [] { return true; });
    printf("số cách đặt 8 hậu = %lld\n", ways);  // 92
    return 0;
}
#endif
