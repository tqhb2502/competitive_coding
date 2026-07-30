#pragma once
#include <bits/stdc++.h>
using namespace std;
// Lazy Segment Tree — ba mẫu sẵn dùng: add/assign+sum, add+min/max, và độ dài hợp các đoạn.
// Khi dùng: CSES 1735 (add/assign/sum), 2425 (add/min/max), 1741 (độ dài đang được phủ khi sweep).
// ĐPT: build O(n); mỗi update/query O(log n); bộ nhớ O(n). Index mảng 0-based, đoạn nửa mở [l,r).
// Dùng: LazySeg s(a); s.update(l,r,d); s.assign(l,r,x); s.query(l,r);
//       RangeAddMinMax mm(a); mm.rangeAdd(l,r,d); CoveredLengthSegTree cover(coords);
// Bẫy: assign GHI ĐÈ action cũ; mọi giá trị/tổng/độ dài phải vừa long long; count phủ không được âm.
// CSES: 1735 1741 2425
struct LazySeg {
    int n;
    vector<long long> sum, lz, assignTag;
    vector<char> hasAssign;
    // Ý nghĩa: dựng cây từ mảng a; cấp phát sum/lz kích thước 4n, build đệ quy nếu n > 0.
    // Tham số: a = mảng giá trị ban đầu (0-based).
    explicit LazySeg(const vector<long long>& a)
        : n((int)a.size()), sum(4 * max(1, n), 0), lz(4 * max(1, n), 0),
          assignTag(4 * max(1, n), 0), hasAssign(4 * max(1, n), false) {
        if (n > 0) build(1, 0, n, a);
    }
    // Ý nghĩa: xây đệ quy nút node phủ đoạn [l, r), gán lá và gộp tổng từ hai con.
    // Tham số: node = chỉ số nút hiện tại; l, r = biên đoạn nửa mở [l, r); a = mảng nguồn.
    void build(int node, int l, int r, const vector<long long>& a) {
        if (r - l == 1) { sum[node] = a[l]; return; }
        int m = (l + r) / 2;
        build(2 * node, l, m, a);
        build(2 * node + 1, m, r, a);
        sum[node] = sum[2 * node] + sum[2 * node + 1];
    }
    // Ý nghĩa: áp phép gán v lên toàn đoạn [l,r), ghi đè mọi action đang chờ.
    // Tham số: node = nút cần áp; l,r = đoạn nút phủ; v = giá trị mới của mọi phần tử.
    void applyAssign(int node, int l, int r, long long v) {
        sum[node] = (long long)((__int128)(r - l) * v);
        assignTag[node] = v;
        hasAssign[node] = true;
        lz[node] = 0;
    }
    // Ý nghĩa: áp phép +v lên toàn đoạn [l, r) của node — cộng vào sum và action đang chờ.
    // Tham số: node = nút cần áp; l, r = biên đoạn nút phủ; v = lượng cộng mỗi phần tử.
    void applyAdd(int node, int l, int r, long long v) {
        sum[node] += (long long)((__int128)(r - l) * v);
        if (hasAssign[node]) assignTag[node] += v;
        else lz[node] += v;
    }
    // Ý nghĩa: đẩy assign trước rồi add xuống hai con, sau đó xóa action tại node.
    // Tham số: node = nút cha; l, r = biên đoạn node phủ (dùng để chia điểm giữa m).
    void push(int node, int l, int r) {
        if (r - l <= 1) return;
        int m = l + (r - l) / 2;
        if (hasAssign[node]) {
            applyAssign(2 * node, l, m, assignTag[node]);
            applyAssign(2 * node + 1, m, r, assignTag[node]);
            hasAssign[node] = false;
        }
        if (lz[node]) {
            applyAdd(2 * node, l, m, lz[node]);
            applyAdd(2 * node + 1, m, r, lz[node]);
            lz[node] = 0;
        }
    }
    // Ý nghĩa: cộng v cho mọi phần tử trong [ql, qr) trên cây con node phủ [l, r), có lazy.
    // Tham số: node = nút hiện tại; l, r = đoạn node phủ; ql, qr = đoạn cần cập nhật [ql, qr); v = lượng cộng.
    void update(int node, int l, int r, int ql, int qr, long long v) {
        if (qr <= l || r <= ql) return;
        if (ql <= l && r <= qr) { applyAdd(node, l, r, v); return; }
        push(node, l, r);
        int m = (l + r) / 2;
        update(2 * node, l, m, ql, qr, v);
        update(2 * node + 1, m, r, ql, qr, v);
        sum[node] = sum[2 * node] + sum[2 * node + 1];
    }
    // Ý nghĩa: gán v cho mọi phần tử thuộc [ql,qr) trên cây con node phủ [l,r).
    // Tham số: node,l,r = nút và đoạn đang xét; ql,qr = đoạn cập nhật; v = giá trị gán.
    void assign(int node, int l, int r, int ql, int qr, long long v) {
        if (qr <= l || r <= ql) return;
        if (ql <= l && r <= qr) { applyAssign(node, l, r, v); return; }
        push(node, l, r);
        int m = l + (r - l) / 2;
        assign(2 * node, l, m, ql, qr, v);
        assign(2 * node + 1, m, r, ql, qr, v);
        sum[node] = sum[2 * node] + sum[2 * node + 1];
    }
    // Ý nghĩa: tính tổng các phần tử trong [ql, qr) trên cây con node phủ [l, r), có đẩy lazy.
    // Tham số: node = nút hiện tại; l, r = đoạn node phủ; ql, qr = đoạn cần truy vấn [ql, qr).
    // Trả về: tổng các phần tử thuộc giao của [l, r) và [ql, qr).
    long long query(int node, int l, int r, int ql, int qr) {
        if (qr <= l || r <= ql) return 0;
        if (ql <= l && r <= qr) return sum[node];
        push(node, l, r);
        int m = (l + r) / 2;
        return query(2 * node, l, m, ql, qr) + query(2 * node + 1, m, r, ql, qr);
    }
    // Ý nghĩa: cộng v cho mọi phần tử trong đoạn [l, r) (gọi bản đệ quy từ gốc).
    // Tham số: l, r = biên đoạn nửa mở [l, r) cần cập nhật; v = lượng cộng mỗi phần tử.
    void update(int l, int r, long long v) {
        checkRange(l, r);
        if (l < r) update(1, 0, n, l, r, v);
    }  // [l, r)
    // Ý nghĩa: gán mọi phần tử trong đoạn nửa mở [l,r) bằng v.
    // Tham số: l,r = biên đoạn hợp lệ; v = giá trị mới.
    void assign(int l, int r, long long v) {
        checkRange(l, r);
        if (l < r) assign(1, 0, n, l, r, v);
    }
    // Ý nghĩa: truy vấn tổng các phần tử trong đoạn [l, r) (gọi bản đệ quy từ gốc).
    // Tham số: l, r = biên đoạn nửa mở [l, r) cần lấy tổng.
    // Trả về: tổng các phần tử trong [l, r).
    long long query(int l, int r) {
        checkRange(l, r);
        return l == r ? 0 : query(1, 0, n, l, r);
    }  // [l, r)

private:
    void checkRange(int l, int r) const {
        if (l < 0 || l > r || r > n) throw out_of_range("LazySeg: đoạn ngoài mảng");
    }
};

// Lazy range-add, range-min/range-max. Hữu ích khi dấu đáp án phụ thuộc min/max toàn mảng.
struct RangeAddMinMax {
    int n;
    vector<long long> minimum, maximum, lazy;

    // Ý nghĩa: dựng cây min/max từ mảng ban đầu.
    // Tham số: values = mảng 0-based.
    explicit RangeAddMinMax(const vector<long long>& values)
        : n((int)values.size()), minimum(4 * max(1, n)), maximum(4 * max(1, n)),
          lazy(4 * max(1, n), 0) {
        if (n > 0) build(1, 0, n, values);
    }

    // Ý nghĩa: xây node phủ [left,right) và hai cực trị.
    // Tham số: node,left,right = nút/đoạn; values = mảng nguồn.
    void build(int node, int left, int right, const vector<long long>& values) {
        if (right - left == 1) {
            minimum[node] = maximum[node] = values[left];
            return;
        }
        int middle = left + (right - left) / 2;
        build(2 * node, left, middle, values);
        build(2 * node + 1, middle, right, values);
        pull(node);
    }

    // Ý nghĩa: cộng delta vào cả node và lazy tag của nó.
    // Tham số: node = nút cần áp; delta = lượng cộng.
    void apply(int node, long long delta) {
        minimum[node] += delta;
        maximum[node] += delta;
        lazy[node] += delta;
    }

    // Ý nghĩa: đẩy lazy add xuống hai con.
    // Tham số: node = nút cha.
    void push(int node) {
        if (lazy[node] == 0) return;
        apply(2 * node, lazy[node]);
        apply(2 * node + 1, lazy[node]);
        lazy[node] = 0;
    }

    // Ý nghĩa: gộp min/max từ hai con.
    // Tham số: node = nút cha.
    void pull(int node) {
        minimum[node] = std::min(minimum[2 * node], minimum[2 * node + 1]);
        maximum[node] = std::max(maximum[2 * node], maximum[2 * node + 1]);
    }

    void rangeAdd(int node, int left, int right, int queryLeft, int queryRight,
                  long long delta) {
        if (queryRight <= left || right <= queryLeft) return;
        if (queryLeft <= left && right <= queryRight) {
            apply(node, delta);
            return;
        }
        push(node);
        int middle = left + (right - left) / 2;
        rangeAdd(2 * node, left, middle, queryLeft, queryRight, delta);
        rangeAdd(2 * node + 1, middle, right, queryLeft, queryRight, delta);
        pull(node);
    }

    pair<long long, long long> query(
            int node, int left, int right, int queryLeft, int queryRight) {
        if (queryRight <= left || right <= queryLeft)
            return {LLONG_MAX, LLONG_MIN};
        if (queryLeft <= left && right <= queryRight)
            return {minimum[node], maximum[node]};
        push(node);
        int middle = left + (right - left) / 2;
        auto a = query(2 * node, left, middle, queryLeft, queryRight);
        auto b = query(2 * node + 1, middle, right, queryLeft, queryRight);
        return {std::min(a.first, b.first), std::max(a.second, b.second)};
    }

    // Ý nghĩa: cộng delta cho mọi phần tử trong [left,right).
    // Tham số: left,right = đoạn nửa mở; delta = lượng cộng.
    void rangeAdd(int left, int right, long long delta) {
        checkRange(left, right);
        if (left < right) rangeAdd(1, 0, n, left, right, delta);
    }

    // Ý nghĩa: lấy cả min và max trên [left,right).
    // Tham số: left,right = đoạn nửa mở KHÔNG RỖNG.
    // Trả về: {min,max}; ném invalid_argument nếu đoạn rỗng.
    pair<long long, long long> rangeMinMax(int left, int right) {
        checkRange(left, right);
        if (left == right) throw invalid_argument("RangeAddMinMax: query rỗng");
        return query(1, 0, n, left, right);
    }

    long long rangeMin(int left, int right) { return rangeMinMax(left, right).first; }
    long long rangeMax(int left, int right) { return rangeMinMax(left, right).second; }
    long long allMin() const {
        if (n == 0) throw logic_error("RangeAddMinMax: cây rỗng");
        return minimum[1];
    }
    long long allMax() const {
        if (n == 0) throw logic_error("RangeAddMinMax: cây rỗng");
        return maximum[1];
    }

private:
    void checkRange(int left, int right) const {
        if (left < 0 || left > right || right > n)
            throw out_of_range("RangeAddMinMax: đoạn ngoài mảng");
    }
};

// Segment tree độ dài phủ trên các elementary interval giữa những tọa độ đã nén.
struct CoveredLengthSegTree {
    vector<long long> xs, minimumCover, lazyCover, lengthAtMinimum;
    int intervals;
    long long totalLength;

    // Ý nghĩa: sort+unique các tọa độ và tạo cây trên [xs[i],xs[i+1]).
    // Tham số: coordinates = mọi đầu mút sẽ xuất hiện, cần ít nhất hai giá trị phân biệt.
    explicit CoveredLengthSegTree(vector<long long> coordinates) {
        sort(coordinates.begin(), coordinates.end());
        coordinates.erase(unique(coordinates.begin(), coordinates.end()), coordinates.end());
        if (coordinates.size() < 2)
            throw invalid_argument("CoveredLengthSegTree: cần >=2 tọa độ phân biệt");
        if ((__int128)coordinates.back() - coordinates.front() > LLONG_MAX)
            throw overflow_error("CoveredLengthSegTree: miền tọa độ quá dài");
        xs = std::move(coordinates);
        intervals = (int)xs.size() - 1;
        totalLength = xs.back() - xs.front();
        minimumCover.assign(4 * intervals, 0);
        lazyCover.assign(4 * intervals, 0);
        lengthAtMinimum.assign(4 * intervals, 0);
        buildCoverage(1, 0, intervals);
    }

    // Ý nghĩa: dựng độ dài vật lý có coverage nhỏ nhất (ban đầu mọi count bằng 0).
    // Tham số: node = nút; left,right = elementary interval index [left,right).
    void buildCoverage(int node, int left, int right) {
        if (right - left == 1) {
            lengthAtMinimum[node] = xs[right] - xs[left];
            return;
        }
        int middle = left + (right - left) / 2;
        buildCoverage(2 * node, left, middle);
        buildCoverage(2 * node + 1, middle, right);
        pull(node);
    }

    // Ý nghĩa: gộp minimum coverage và tổng độ dài đạt minimum từ hai con.
    // Tham số: node = nút cha.
    void pull(int node) {
        minimumCover[node] =
            min(minimumCover[2 * node], minimumCover[2 * node + 1]);
        lengthAtMinimum[node] = 0;
        if (minimumCover[2 * node] == minimumCover[node])
            lengthAtMinimum[node] += lengthAtMinimum[2 * node];
        if (minimumCover[2 * node + 1] == minimumCover[node])
            lengthAtMinimum[node] += lengthAtMinimum[2 * node + 1];
    }

    // Ý nghĩa: cộng delta lazy lên toàn node, có kiểm tra tràn count.
    // Tham số: node = nút cần áp; delta = lượng đổi coverage.
    void applyCoverage(int node, long long delta) {
        __int128 nextMinimum = (__int128)minimumCover[node] + delta;
        __int128 nextLazy = (__int128)lazyCover[node] + delta;
        if (nextMinimum < LLONG_MIN || nextMinimum > LLONG_MAX ||
            nextLazy < LLONG_MIN || nextLazy > LLONG_MAX)
            throw overflow_error("CoveredLengthSegTree: coverage count tràn");
        minimumCover[node] = (long long)nextMinimum;
        lazyCover[node] = (long long)nextLazy;
    }

    // Ý nghĩa: đẩy range-add coverage xuống hai con.
    // Tham số: node = nút cha.
    void pushCoverage(int node) {
        if (lazyCover[node] == 0) return;
        applyCoverage(2 * node, lazyCover[node]);
        applyCoverage(2 * node + 1, lazyCover[node]);
        lazyCover[node] = 0;
    }

    void add(int node, int left, int right, int queryLeft, int queryRight,
             long long delta) {
        if (queryRight <= left || right <= queryLeft) return;
        if (queryLeft <= left && right <= queryRight) {
            applyCoverage(node, delta);
            return;
        }
        pushCoverage(node);
        int middle = left + (right - left) / 2;
        add(2 * node, left, middle, queryLeft, queryRight, delta);
        add(2 * node + 1, middle, right, queryLeft, queryRight, delta);
        pull(node);
    }

    // Ý nghĩa: lấy coverage nhỏ nhất trong đoạn index query, dùng để chặn update làm count âm.
    // Tham số: node,left,right = cây con; queryLeft,queryRight = đoạn cần hỏi.
    // Trả về: minimum coverage trên phần giao; LLONG_MAX nếu không giao.
    long long rangeMinimum(int node, int left, int right,
                           int queryLeft, int queryRight) {
        if (queryRight <= left || right <= queryLeft) return LLONG_MAX;
        if (queryLeft <= left && right <= queryRight) return minimumCover[node];
        pushCoverage(node);
        int middle = left + (right - left) / 2;
        return min(rangeMinimum(2 * node, left, middle, queryLeft, queryRight),
                   rangeMinimum(2 * node + 1, middle, right,
                                queryLeft, queryRight));
    }

    // Ý nghĩa: cộng delta vào số lớp phủ của elementary interval index [left,right).
    // Tham số: left,right = index 0-based; delta thường là +1/-1.
    void addIndexRange(int left, int right, int delta) {
        if (left < 0 || left > right || right > intervals)
            throw out_of_range("CoveredLengthSegTree: đoạn index ngoài miền");
        if (left == right || delta == 0) return;
        if (delta < 0 &&
            (__int128)rangeMinimum(1, 0, intervals, left, right) + delta < 0)
            throw logic_error("CoveredLengthSegTree: count phủ âm");
        add(1, 0, intervals, left, right, delta);
    }

    // Ý nghĩa: cộng delta độ phủ trên đoạn tọa độ nửa mở [leftCoordinate,rightCoordinate).
    // Tham số: hai đầu mút phải có trong bảng coordinates truyền vào constructor.
    void addCoordinateRange(long long leftCoordinate, long long rightCoordinate, int delta) {
        auto left = lower_bound(xs.begin(), xs.end(), leftCoordinate);
        auto right = lower_bound(xs.begin(), xs.end(), rightCoordinate);
        if (left == xs.end() || *left != leftCoordinate ||
            right == xs.end() || *right != rightCoordinate)
            throw invalid_argument("CoveredLengthSegTree: đầu mút chưa được nén");
        if (leftCoordinate > rightCoordinate)
            throw invalid_argument("CoveredLengthSegTree: đoạn ngược");
        addIndexRange((int)(left - xs.begin()), (int)(right - xs.begin()), delta);
    }

    // Ý nghĩa: lấy tổng độ dài vật lý đang được phủ ít nhất một lần.
    // Trả về: độ dài phủ trên toàn miền.
    long long coveredLength() const {
        assert(minimumCover[1] >= 0);
        return minimumCover[1] > 0
            ? totalLength
            : totalLength - lengthAtMinimum[1];
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ lazy-segment-tree.hpp -o demo && ./demo
int main() {
    vector<long long> a = {1, 2, 3, 4, 5};
    LazySeg st(a);
    st.update(1, 4, 10);   // +10 tren [1,4)
    st.assign(2, 4, 7);
    RangeAddMinMax extrema(a);
    extrema.rangeAdd(0, 3, -4);
    CoveredLengthSegTree cover({0, 2, 5, 9});
    cover.addCoordinateRange(0, 5, 1);
    cover.addCoordinateRange(2, 9, 1);
    printf("sum=%lld min=%lld max=%lld covered=%lld\n",
           st.query(0, 5), extrema.allMin(), extrema.allMax(), cover.coveredLength());
    return 0;
}
#endif
