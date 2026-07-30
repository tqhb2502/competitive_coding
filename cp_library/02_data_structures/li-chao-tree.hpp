#pragma once
#include <bits/stdc++.h>
using namespace std;
// Li Chao Tree động — thêm đường y=a*x+b trên toàn miền hoặc một đoạn, hỏi min/max tại x nguyên.
// Khi dùng: tối ưu DP với slope/query bất kỳ; addSegment dùng khi mỗi đường chỉ hợp lệ trên một khoảng x.
// ĐPT: addLine/query O(log C), addSegment O(log^2 C); bộ nhớ theo số node được chạm. Miền [lo,hi] đóng.
// Dùng: LiChao lc(lo,hi); lc.addLine(a,b); lc.addSegment(a,b,l,r); lc.query(x);
// Bẫy: query ngoài miền/rỗng và kết quả không vừa long long sẽ ném exception; hệ số vẫn là long long.
// CSES: 2085 3227 3429 3430
struct LiChao {
    using Wide = __int128;
    enum class Objective { Minimum, Maximum };

    struct Line {
        long long a = 0, b = 0;
    };
    struct Node {
        Line line;
        int left = -1, right = -1;
        bool hasLine = false;
    };

    vector<Node> tree;
    long long domainLeft, domainRight;
    Objective objective;

    // Ý nghĩa: tạo cây rỗng trên miền số nguyên đóng [left,right], cho bài toán min hoặc max.
    // Tham số: left,right = biên miền; objective = loại cực trị cần lấy.
    explicit LiChao(long long left, long long right,
                    Objective objective = Objective::Minimum)
        : domainLeft(left), domainRight(right), objective(objective) {
        if (left > right) throw invalid_argument("LiChao: miền rỗng");
        tree.push_back(Node());
    }

    // Ý nghĩa: thêm đường y = slope*x + intercept, hợp lệ trên toàn miền.
    // Tham số: slope = hệ số góc; intercept = hệ số tự do.
    void addLine(long long slope, long long intercept) {
        addLineAt(0, domainLeft, domainRight, {slope, intercept});
    }

    // Ý nghĩa: thêm đường chỉ trên đoạn số nguyên đóng [left,right].
    // Tham số: slope,intercept = đường thẳng; left,right = đoạn hợp lệ nằm trong miền cây.
    void addSegment(long long slope, long long intercept,
                    long long left, long long right) {
        checkSegment(left, right);
        addSegmentAt(0, domainLeft, domainRight, left, right, {slope, intercept});
    }

    // Ý nghĩa: hỏi cực trị chính xác bằng __int128, cho phép phân biệt cây rỗng tại x.
    // Tham số: x = hoành độ nguyên trong miền constructor.
    // Trả về: nullopt nếu chưa có đường nào hợp lệ tại x; ngược lại là giá trị min/max.
    optional<Wide> queryWideIfAny(long long x) const {
        checkX(x);
        return queryAt(0, domainLeft, domainRight, x);
    }

    // Ý nghĩa: hỏi cực trị dưới dạng long long và phân biệt trường hợp không có đường.
    // Tham số: x = hoành độ nguyên trong miền.
    // Trả về: nullopt nếu rỗng tại x; ném overflow_error nếu kết quả không vừa long long.
    optional<long long> queryIfAny(long long x) const {
        auto result = queryWideIfAny(x);
        if (!result) return nullopt;
        if (*result < (Wide)LLONG_MIN || *result > (Wide)LLONG_MAX)
            throw overflow_error("LiChao: kết quả vượt long long");
        return (long long)*result;
    }

    // Ý nghĩa: hỏi cực trị tại x khi chắc chắn có ít nhất một đường hợp lệ.
    // Tham số: x = hoành độ nguyên trong miền.
    // Trả về: min/max tại x; ném logic_error nếu rỗng, overflow_error nếu không vừa long long.
    long long query(long long x) const {
        auto result = queryIfAny(x);
        if (!result) throw logic_error("LiChao: không có đường hợp lệ tại x");
        return *result;
    }

private:
    static Wide evaluate(const Line& line, long long x) {
        return (Wide)line.a * x + line.b;
    }

    static long long midpoint(long long left, long long right) {
        Wide distance = (Wide)right - left;
        return (long long)((Wide)left + distance / 2);
    }

    bool better(Wide first, Wide second) const {
        return objective == Objective::Minimum ? first < second : first > second;
    }

    int ensureLeft(int node) {
        if (tree[node].left == -1) {
            int child = (int)tree.size();
            tree[node].left = child;
            tree.push_back(Node());
        }
        return tree[node].left;
    }

    int ensureRight(int node) {
        if (tree[node].right == -1) {
            int child = (int)tree.size();
            tree[node].right = child;
            tree.push_back(Node());
        }
        return tree[node].right;
    }

    void addLineAt(int node, long long left, long long right, Line candidate) {
        if (!tree[node].hasLine) {
            tree[node].line = candidate;
            tree[node].hasLine = true;
            return;
        }

        // Hai slope bằng nhau: giữ đúng đường có intercept tốt hơn, không sinh node vô ích.
        if (candidate.a == tree[node].line.a) {
            if (better((Wide)candidate.b, (Wide)tree[node].line.b))
                tree[node].line = candidate;
            return;
        }

        long long middle = midpoint(left, right);
        bool betterAtLeft =
            better(evaluate(candidate, left), evaluate(tree[node].line, left));
        bool betterAtMiddle =
            better(evaluate(candidate, middle), evaluate(tree[node].line, middle));
        if (betterAtMiddle) swap(candidate, tree[node].line);
        if (left == right) return;

        if (betterAtLeft != betterAtMiddle) {
            int child = ensureLeft(node);
            addLineAt(child, left, middle, candidate);
        } else {
            int child = ensureRight(node);
            addLineAt(child, middle + 1, right, candidate);
        }
    }

    void addSegmentAt(int node, long long left, long long right,
                      long long queryLeft, long long queryRight, Line line) {
        if (queryRight < left || right < queryLeft) return;
        if (queryLeft <= left && right <= queryRight) {
            addLineAt(node, left, right, line);
            return;
        }
        long long middle = midpoint(left, right);
        if (queryLeft <= middle) {
            int child = ensureLeft(node);
            addSegmentAt(child, left, middle, queryLeft, queryRight, line);
        }
        if (middle < queryRight) {
            int child = ensureRight(node);
            addSegmentAt(child, middle + 1, right, queryLeft, queryRight, line);
        }
    }

    optional<Wide> queryAt(int node, long long left, long long right,
                           long long x) const {
        optional<Wide> result;
        if (tree[node].hasLine) result = evaluate(tree[node].line, x);
        if (left == right) return result;

        long long middle = midpoint(left, right);
        int child = x <= middle ? tree[node].left : tree[node].right;
        if (child == -1) return result;
        auto below = x <= middle
            ? queryAt(child, left, middle, x)
            : queryAt(child, middle + 1, right, x);
        if (below && (!result || better(*below, *result))) result = below;
        return result;
    }

    void checkX(long long x) const {
        if (x < domainLeft || x > domainRight)
            throw out_of_range("LiChao: x ngoài miền");
    }

    void checkSegment(long long left, long long right) const {
        if (left > right)
            throw invalid_argument("LiChao: đoạn ngược");
        if (left < domainLeft || right > domainRight)
            throw out_of_range("LiChao: đoạn ngoài miền");
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ li-chao-tree.hpp -o demo && ./demo
int main() {
    LiChao minimum(-100, 100);
    minimum.addLine(2, 3);                 // 2x+3
    minimum.addLine(-1, 5);                // -x+5
    minimum.addSegment(0, -20, -2, 2);    // chỉ cạnh tranh gần gốc
    LiChao maximum(-100, 100, LiChao::Objective::Maximum);
    maximum.addLine(2, 3);
    maximum.addLine(-1, 5);
    printf("min(0)=%lld min(10)=%lld max(10)=%lld\n",
           minimum.query(0), minimum.query(10), maximum.query(10));  // -20 -5 23
    return 0;
}
#endif
