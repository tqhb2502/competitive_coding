#pragma once
#include <bits/stdc++.h>
using namespace std;
// Suffix Automaton (SAM) — nhận mọi xâu con; đếm/vị trí xuất hiện và chọn xâu con thứ k theo từ điển.
// Khi dùng: đếm xâu con phân biệt; tìm pattern sớm nhất; số lần xuất hiện; xâu con thứ k (phân biệt hoặc tính cả lặp).
// ĐPT: dựng O(n * A), A=26; propagate/DP O(n * A); mỗi pattern O(|p|), truy vết O(|đáp án| * A); bộ nhớ O(n * A).
// Dùng: SuffixAutomaton sam; sam.add(s); sam.firstOccurrence(p); sam.occurrenceCount(p); sam.kthDistinctSubstring(k);
// Bẫy: chỉ nhận 'a'..'z'; k là 1-based, trả "" nếu ngoài miền; gọi extend sau propagate vẫn an toàn nhưng phải truy vấn lại.
// CSES: 2102 2103 2104 2105 2106 2108 2109 2110
struct SuffixAutomaton {
    struct State {
        int len, link;
        array<int, 26> nxt;
        long long occ;       // kích thước endpos sau propagateOccurrences()
        int min_end;         // vị trí kết thúc nhỏ nhất (0-based), INT_MAX ở gốc
        long long terminal;  // 1 ở state tiền tố thật, 0 ở clone/gốc
    };

    vector<State> st;
    int last;
    bool occurrences_ready;

    // Ý nghĩa: khởi tạo SAM với trạng thái gốc (xâu rỗng).
    SuffixAutomaton() : last(0), occurrences_ready(false) {
        State root{0, -1, {}, 0, INT_MAX, 0};
        root.nxt.fill(-1);
        st.push_back(root);
    }

    // Ý nghĩa: tạo SAM và nạp ngay toàn bộ xâu s.
    // Tham số: s = xâu chỉ gồm chữ thường 'a'..'z'.
    explicit SuffixAutomaton(const string& s) : SuffixAutomaton() { add(s); }

    // Ý nghĩa: mở rộng SAM thêm một ký tự ở cuối xâu; dữ liệu occurrence đã tính trước đó sẽ được đánh dấu cần tính lại.
    // Tham số: c = mã ký tự trong [0, 26).
    void extend(int c) {
        assert(0 <= c && c < 26);
        int cur = (int)st.size();
        State ns{st[last].len + 1, -1, {}, 1, st[last].len, 1};
        ns.nxt.fill(-1);
        st.push_back(ns);

        int p = last;
        while (p != -1 && st[p].nxt[c] == -1) {
            st[p].nxt[c] = cur;
            p = st[p].link;
        }
        if (p == -1) {
            st[cur].link = 0;
        } else {
            int q = st[p].nxt[c];
            if (st[p].len + 1 == st[q].len) {
                st[cur].link = q;
            } else {
                State clone_state = st[q];  // sao chép trước push_back để không giữ tham chiếu treo
                clone_state.len = st[p].len + 1;
                clone_state.occ = 0;
                clone_state.terminal = 0;
                int clone = (int)st.size();
                st.push_back(clone_state);
                while (p != -1 && st[p].nxt[c] == q) {
                    st[p].nxt[c] = clone;
                    p = st[p].link;
                }
                st[q].link = clone;
                st[cur].link = clone;
            }
        }
        last = cur;
        occurrences_ready = false;
    }

    // Ý nghĩa: thêm cả xâu s vào SAM hiện tại, tương đương nối s vào cuối phần đã nạp.
    // Tham số: s = xâu chỉ gồm chữ thường 'a'..'z'.
    void add(const string& s) {
        st.reserve(st.size() + 2 * s.size());
        for (char ch : s) extend(ch - 'a');
    }

    // Ý nghĩa: lấy thứ tự các state tăng dần theo len bằng counting sort.
    // Trả về: vector chứa mọi chỉ số state; suffix link luôn đi từ phần tử sau về phần tử trước.
    vector<int> statesByLength() const {
        int max_len = 0;
        for (const State& state : st) max_len = max(max_len, state.len);
        vector<int> count(max_len + 1, 0), order(st.size());
        for (const State& state : st) count[state.len]++;
        for (int i = 1; i <= max_len; i++) count[i] += count[i - 1];
        for (int v = (int)st.size() - 1; v >= 0; v--) order[--count[st[v].len]] = v;
        return order;
    }

    // Ý nghĩa: lan số state tiền tố thật ngược qua suffix link để tính kích thước endpos của mọi state.
    void propagateOccurrences() {
        for (State& state : st) state.occ = state.terminal;
        vector<int> order = statesByLength();
        for (int i = (int)order.size() - 1; i > 0; i--) {
            int v = order[i];
            st[st[v].link].occ += st[v].occ;
        }
        occurrences_ready = true;
    }

    // Ý nghĩa: chạy pattern từ gốc trên automaton.
    // Tham số: pattern = xâu chỉ gồm 'a'..'z'.
    // Trả về: state kết thúc, hoặc -1 nếu pattern không phải xâu con.
    int matchState(const string& pattern) const {
        int v = 0;
        for (char ch : pattern) {
            int c = ch - 'a';
            if (c < 0 || c >= 26 || st[v].nxt[c] == -1) return -1;
            v = st[v].nxt[c];
        }
        return v;
    }

    // Ý nghĩa: tìm lần xuất hiện có vị trí bắt đầu nhỏ nhất của pattern.
    // Tham số: pattern = xâu cần tìm.
    // Trả về: vị trí bắt đầu 0-based; -1 nếu không có; pattern rỗng trả 0.
    int firstOccurrence(const string& pattern) const {
        if (pattern.empty()) return 0;
        int v = matchState(pattern);
        return v == -1 ? -1 : st[v].min_end - (int)pattern.size() + 1;
    }

    // Ý nghĩa: đếm số lần xuất hiện (cho phép chồng lấn) của pattern trong xâu đã nạp.
    // Tham số: pattern = xâu cần đếm.
    // Trả về: kích thước endpos của pattern; 0 nếu không có; xâu rỗng trả n+1.
    long long occurrenceCount(const string& pattern) {
        if (pattern.empty()) return (long long)st[last].len + 1;
        int v = matchState(pattern);
        if (v == -1) return 0;
        if (!occurrences_ready) propagateOccurrences();
        return st[v].occ;
    }

    // Ý nghĩa: đếm số xâu con phân biệt khác rỗng của xâu đã nạp.
    // Trả về: tổng len[v] - len[link[v]] trên mọi state khác gốc.
    long long distinctSubstrings() const {
        long long total = 0;
        for (int v = 1; v < (int)st.size(); v++) total += st[v].len - st[st[v].link].len;
        return total;
    }

    // Ý nghĩa: lấy xâu con PHÂN BIỆT thứ k trong thứ tự từ điển.
    // Tham số: k = thứ hạng 1-based.
    // Trả về: xâu cần tìm, hoặc "" nếu k <= 0 hay k lớn hơn số xâu con phân biệt.
    string kthDistinctSubstring(long long k) const {
        if (k <= 0) return "";
        vector<int> order = statesByLength();
        vector<long long> paths(st.size(), 0);
        auto saturated_add = [](long long a, long long b) {
            return a > LLONG_MAX - b ? LLONG_MAX : a + b;
        };
        for (int at = (int)order.size() - 1; at >= 0; at--) {
            int v = order[at];
            for (int c = 0; c < 26; c++) {
                int u = st[v].nxt[c];
                if (u != -1) paths[v] = saturated_add(paths[v], saturated_add(1, paths[u]));
            }
        }
        if (k > paths[0]) return "";

        string answer;
        int v = 0;
        while (true) {
            for (int c = 0; c < 26; c++) {
                int u = st[v].nxt[c];
                if (u == -1) continue;
                long long block = saturated_add(1, paths[u]);
                if (k > block) {
                    k -= block;
                    continue;
                }
                answer.push_back(char('a' + c));
                if (k == 1) return answer;
                k--;
                v = u;
                break;
            }
        }
    }

    // Ý nghĩa: lấy xâu con thứ k theo từ điển khi mỗi xâu được lặp lại đúng số lần nó xuất hiện.
    // Tham số: k = thứ hạng 1-based trong danh sách n(n+1)/2 xâu con có tính lặp.
    // Trả về: xâu cần tìm, hoặc "" nếu k ngoài miền.
    string kthSubstringByMultiplicity(long long k) {
        if (k <= 0) return "";
        if (!occurrences_ready) propagateOccurrences();
        vector<int> order = statesByLength();
        vector<long long> paths(st.size(), 0);
        auto saturated_add = [](long long a, long long b) {
            return a > LLONG_MAX - b ? LLONG_MAX : a + b;
        };
        for (int at = (int)order.size() - 1; at >= 0; at--) {
            int v = order[at];
            for (int c = 0; c < 26; c++) {
                int u = st[v].nxt[c];
                if (u != -1) paths[v] = saturated_add(paths[v], saturated_add(st[u].occ, paths[u]));
            }
        }
        if (k > paths[0]) return "";

        string answer;
        int v = 0;
        while (true) {
            for (int c = 0; c < 26; c++) {
                int u = st[v].nxt[c];
                if (u == -1) continue;
                long long block = saturated_add(st[u].occ, paths[u]);
                if (k > block) {
                    k -= block;
                    continue;
                }
                answer.push_back(char('a' + c));
                if (k <= st[u].occ) return answer;
                k -= st[u].occ;
                v = u;
                break;
            }
        }
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ suffix-automaton.hpp -o demo && ./demo
int main() {
    SuffixAutomaton sam;
    sam.add("abab");
    printf("phân biệt=%lld; first('ba')=%d; occ('ab')=%lld\n",
           sam.distinctSubstrings(), sam.firstOccurrence("ba"), sam.occurrenceCount("ab"));  // 7; 1; 2
    printf("distinct #4=%s; multiplicity #5=%s\n",
           sam.kthDistinctSubstring(4).c_str(), sam.kthSubstringByMultiplicity(5).c_str());   // abab; aba
    return 0;
}
#endif
