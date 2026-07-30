#pragma once
#include <bits/stdc++.h>
using namespace std;
// KMP automaton — bảng chuyển trạng thái (DFA) dựng từ hàm tiền tố, phục vụ DP trên xâu theo một mẫu cố định.
// Khi dùng: đếm xâu độ dài n KHÔNG chứa (hoặc CÓ chứa) một mẫu; DP với trạng thái "đã khớp bao nhiêu ký tự đầu của mẫu"; theo dõi số lần mẫu xuất hiện khi ghép dần từng ký tự.
// ĐPT: kmpAutomaton O(|p| * |alphabet|) thời gian và bộ nhớ; countStringsAvoiding O(n * |p| * (d + 1)) thời gian, O(|p| * (d + 1)) bộ nhớ, với d = số ký tự PHÂN BIỆT trong mẫu (d <= |p| và d <= alphabetSize; ví dụ n = 1000, |p| = 100, 26 chữ cái: ~2.6M phép).
// Dùng: auto nxt = kmpAutomaton("ABABC", "ABC");                 // nxt[j][c] = trạng thái mới khi đang ở j và đọc alphabet[c]
//       long long avoid = countStringsAvoiding(5, "ABC", 26, 1000000007LL);   // CSES 1112: đáp án = (26^n - avoid) mod
// Bẫy: trạng thái chạy 0..|p| nên bảng có |p| + 1 HÀNG; hàng |p| (vừa khớp trọn mẫu) vẫn có cạnh đi tiếp theo fail link nên đếm được lần khớp CHỒNG LẤN — muốn chặn thì tự bỏ trạng thái |p| như countStringsAvoiding làm;
//      cột đánh theo CHỈ SỐ trong alphabet (không phải mã ký tự); alphabet nên liệt kê mỗi ký tự một lần và chứa mọi ký tự của mẫu (nếu thiếu, automaton không bao giờ tới trạng thái |p|);
//      countStringsAvoiding cần mod >= 1 (cộng dồn đã chống tràn nên mod tới ~9.2e18 vẫn đúng) và MỌI ký tự của mẫu phải thuộc đúng bảng chữ cái đang đếm — hàm chỉ nhận alphabetSize nên không kiểm được: gọi với mẫu "xy" và alphabetSize = 2 sẽ hiểu {x,y} LÀ bảng chữ cái (ra 6 với n = 5) chứ không trả 2^5;
//      mẫu rỗng trả 0 (mọi xâu đều chứa xâu rỗng), n = 0 trả 1 % mod, |p| > n hoặc d > alphabetSize cho alphabetSize^n mod mod.
// CSES: 1112
// Ý nghĩa: dựng bảng chuyển của KMP automaton từ hàm tiền tố của mẫu; trạng thái j = "j ký tự đầu của mẫu là hậu tố dài nhất của xâu đã đọc", trạng thái |pattern| = mẫu vừa xuất hiện.
// Tham số: pattern = mẫu (có thể rỗng); alphabet = các ký tự của bảng chữ cái, mỗi ký tự một lần, thứ tự này chính là thứ tự cột.
// Trả về: bảng nxt kích thước (|pattern| + 1) x |alphabet|, chỉ số 0-based cả hai chiều: nxt[j][c] = trạng thái sau khi thêm ký tự alphabet[c] vào xâu đang ở trạng thái j.
vector<vector<int>> kmpAutomaton(const string& pattern, const string& alphabet) {
    int m = (int)pattern.size(), k = (int)alphabet.size();
    vector<int> pi(m, 0);                       // hàm tiền tố của mẫu (tính tại chỗ để file tự chứa)
    for (int i = 1; i < m; i++) {
        int j = pi[i - 1];
        while (j && pattern[i] != pattern[j]) j = pi[j - 1];
        if (pattern[i] == pattern[j]) j++;
        pi[i] = j;
    }
    vector<vector<int>> nxt(m + 1, vector<int>(k, 0));
    for (int j = 0; j <= m; j++)
        for (int c = 0; c < k; c++) {
            if (j < m && alphabet[c] == pattern[j]) nxt[j][c] = j + 1;     // khớp thêm một ký tự
            else nxt[j][c] = (j == 0 ? 0 : nxt[pi[j - 1]][c]);             // lùi theo fail link (pi[j-1] < j nên đã tính xong)
        }
    return nxt;
}
// Ý nghĩa: đếm số xâu độ dài n trên bảng chữ cái alphabetSize ký tự mà KHÔNG chứa pattern như xâu con liên tiếp, bằng DP trên KMP automaton (bỏ mọi đường đi chạm trạng thái |pattern|).
// Tham số: n = độ dài xâu cần đếm (n >= 0); pattern = mẫu phải tránh, mọi ký tự của mẫu phải là ký tự của bảng chữ cái (hàm chỉ dùng SỐ ký tự phân biệt của mẫu); alphabetSize = số ký tự của bảng chữ cái (>= 0); mod = modulo (>= 1).
// Trả về: số xâu tránh được mẫu, đã lấy dư mod (0 nếu pattern rỗng; alphabetSize^n nếu mẫu dùng nhiều ký tự phân biệt hơn alphabetSize, tức mẫu không thể xuất hiện).
long long countStringsAvoiding(int n, const string& pattern, int alphabetSize, long long mod) {
    int m = (int)pattern.size();
    if (m == 0) return 0;                       // xâu rỗng là xâu con của mọi xâu
    string used;                                // các ký tự phân biệt của mẫu, theo thứ tự gặp
    for (char ch : pattern)
        if (used.find(ch) == string::npos) used.push_back(ch);
    int d = (int)used.size();
    if (d > alphabetSize) {                     // mẫu cần nhiều ký tự hơn bảng chữ cái -> không xâu nào chứa được, đếm tất
        long long all = 1 % mod;
        for (int i = 0; i < n; i++) all = (long long)((__int128)all * alphabetSize % mod);
        return all;
    }
    // Ký tự KHÔNG xuất hiện trong mẫu luôn đưa automaton về trạng thái 0, nên gộp cả nhóm đó thành một lớp có others phần tử.
    long long others = (long long)alphabetSize - d;
    vector<vector<int>> nxt = kmpAutomaton(pattern, used);
    // Cộng theo mod KHÔNG tạo tổng trung gian: a + b tràn long long khi mod > 4.6e18 (~2^62).
    auto addMod = [&](long long a, long long b) { return b >= mod - a ? b - (mod - a) : a + b; };
    vector<long long> dp(m, 0), cur(m, 0);      // dp[j] = số xâu chưa từng chạm trạng thái m và đang ở trạng thái j
    dp[0] = 1 % mod;                            // xâu rỗng
    for (int step = 0; step < n; step++) {
        fill(cur.begin(), cur.end(), 0LL);
        for (int j = 0; j < m; j++) {
            long long w = dp[j];
            if (w == 0) continue;
            for (int c = 0; c < d; c++) {
                int t = nxt[j][c];
                if (t < m) cur[t] = addMod(cur[t], w);    // t == m nghĩa là mẫu vừa xuất hiện -> loại
            }
            if (others) cur[0] = addMod(cur[0], (long long)((__int128)w * others % mod));
        }
        dp.swap(cur);
    }
    long long res = 0;
    for (long long w : dp) res = addMod(res, w);
    return res;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ kmp-automaton.hpp -o demo && ./demo
int main() {
    const long long MODV = 1000000007LL;
    auto nxt = kmpAutomaton("ABABC", "ABC");     // cột 0='A', 1='B', 2='C'; trạng thái 0..5
    printf("nxt[4][A]=%d nxt[4][C]=%d nxt[5][A]=%d\n", nxt[4][0], nxt[4][2], nxt[5][0]);   // 3 5 1

    // Đếm lần xuất hiện CHỒNG LẤN của "ABAB" trong "ABABAB" nhờ hàng cuối của bảng.
    string pat = "ABAB", text = "ABABAB", alpha = "AB";
    auto tab = kmpAutomaton(pat, alpha);
    auto colOf = [&](char ch) { return (int)alpha.find(ch); };
    int state = 0, hits = 0;
    for (char ch : text) { state = tab[state][colOf(ch)]; if (state == (int)pat.size()) hits++; }
    printf("so lan 'ABAB' trong 'ABABAB' = %d\n", hits);   // 2

    printf("do dai 3 tren {a,b} tranh \"ab\" = %lld\n", countStringsAvoiding(3, "ab", 2, MODV));   // 4
    printf("do dai 2 tren {a,b,c} tranh \"a\" = %lld\n", countStringsAvoiding(2, "a", 3, MODV));   // 4
    printf("n=0 -> %lld ; mau dai hon n -> %lld\n",
           countStringsAvoiding(0, "ab", 2, MODV), countStringsAvoiding(1, "ab", 2, MODV));        // 1 ; 2

    // CSES 1112 (Required Substring): đếm xâu độ dài n trên 'A'..'Z' CÓ chứa mẫu = 26^n - (số xâu tránh mẫu).
    int n = 5;
    string p = "ABC";
    long long total = 1;
    for (int i = 0; i < n; i++) total = total * 26 % MODV;
    long long contains = (total - countStringsAvoiding(n, p, 26, MODV) % MODV + MODV) % MODV;
    printf("CSES 1112 n=5 p=ABC -> %lld\n", contains);   // 2028
    return 0;
}
#endif
