#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];

    // Nén tọa độ: giữ nguyên thứ tự nên id nhỏ hơn <=> giá trị gốc nhỏ hơn,
    // biến tie-break "giá trị nhỏ nhất" thành "id nén nhỏ nhất".
    vector<int> vals(a);
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
    int m = (int)vals.size();
    vector<int> id(n);
    for (int i = 0; i < n; i++)
        id[i] = int(lower_bound(vals.begin(), vals.end(), a[i]) - vals.begin());

    vector<int> cnt(m, 0);           // tần suất của từng id nén trong cửa sổ
    vector<set<int>> freqSet(k + 1); // freqSet[f] = tập id có tần suất đúng bằng f
    int maxFreq = 0;                 // tần suất lớn nhất đang có trong cửa sổ

    // Thêm một phần tử: dời x lên mức tần suất cao hơn 1 và cập nhật maxFreq.
    auto addElem = [&](int x) {
        int c = cnt[x];
        if (c > 0) freqSet[c].erase(x);
        c++;
        cnt[x] = c;
        freqSet[c].insert(x);
        if (c > maxFreq) maxFreq = c;
    };
    // Bớt một phần tử: dời x xuống mức tần suất thấp hơn 1; maxFreq giảm nhiều
    // nhất 1 đơn vị nên chỉ cần kiểm tra mức hiện tại có rỗng hay không.
    auto removeElem = [&](int x) {
        int c = cnt[x];
        freqSet[c].erase(x);
        c--;
        cnt[x] = c;
        if (c > 0) freqSet[c].insert(x);
        if (freqSet[maxFreq].empty()) maxFreq--;
    };

    // Dựng cửa sổ đầu tiên gồm k phần tử.
    for (int i = 0; i < k; i++) addElem(id[i]);

    // Mode của cửa sổ = giá trị gốc ứng với id nhỏ nhất ở mức maxFreq.
    string out;
    out.reserve((size_t)(n - k + 1) * 11);
    out += to_string(vals[*freqSet[maxFreq].begin()]);

    // Trượt cửa sổ: bỏ phần tử bên trái, thêm phần tử bên phải, rồi ghi mode.
    for (int i = k; i < n; i++) {
        removeElem(id[i - k]);
        addElem(id[i]);
        out += ' ';
        out += to_string(vals[*freqSet[maxFreq].begin()]);
    }
    out += '\n';

    cout << out;
    return 0;
}
