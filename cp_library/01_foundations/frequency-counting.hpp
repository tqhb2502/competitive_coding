#pragma once
#include <bits/stdc++.h>
using namespace std;
// Frequency Counting — bảng tần suất hash, run-length encoding và mex của tập số không âm.
// Khi dùng: đếm phần tử phân biệt/số lần xuất hiện; xử lý alphabet; gom đoạn bằng nhau; tìm mex.
// ĐPT: frequencyTable kỳ vọng O(n), RLE O(n), mex O(n); bộ nhớ O(số giá trị phân biệt hoặc n).
// Dùng: auto f=frequencyTable(a); auto runs=runLengthEncode(s); int m=mexNonnegative(a);
// Bẫy: unordered_map có worst-case O(n²); cần custom hash khi input đối kháng; mex chỉ xét giá trị trong [0,n].
// CSES: 1069 1087 1621 1622 1743 1755 3421
// Ý nghĩa: lập bảng giá trị -> số lần xuất hiện bằng unordered_map.
// Tham số: values = dãy đầu vào; Hash = hàm hash (có thể truyền custom hash chống hack).
// Trả về: unordered_map chứa tần suất mỗi giá trị.
template <class T, class Hash = hash<T>>
unordered_map<T, int, Hash> frequencyTable(const vector<T>& values, Hash hasher = Hash{}) {
    unordered_map<T, int, Hash> freq(0, hasher);
    freq.reserve(values.size() * 2 + 1);
    freq.max_load_factor(0.7f);
    for (const T& value : values) ++freq[value];
    return freq;
}

// Ý nghĩa: nén dãy thành các cặp (giá trị, độ dài run liên tiếp).
// Tham số: values = dãy bất kỳ có toán tử ==.
// Trả về: các run theo đúng thứ tự xuất hiện.
template <class T>
vector<pair<T, int>> runLengthEncode(const vector<T>& values) {
    vector<pair<T, int>> runs;
    for (const T& value : values) {
        if (runs.empty() || !(runs.back().first == value)) runs.push_back({value, 1});
        else ++runs.back().second;
    }
    return runs;
}

// Ý nghĩa: overload RLE tiện dụng cho string.
// Tham số: text = xâu đầu vào.
// Trả về: các cặp (ký tự, độ dài run).
vector<pair<char, int>> runLengthEncode(const string& text) {
    return runLengthEncode(vector<char>(text.begin(), text.end()));
}

// Ý nghĩa: tìm mex nhỏ nhất không âm của dãy.
// Tham số: values = dãy số nguyên (âm và >n tự động bỏ qua).
// Trả về: số nhỏ nhất >=0 không xuất hiện.
int mexNonnegative(const vector<int>& values) {
    vector<char> seen(values.size() + 1, false);
    for (int x : values) if (0 <= x && x <= (int)values.size()) seen[x] = true;
    for (int x = 0; x <= (int)values.size(); ++x) if (!seen[x]) return x;
    return (int)values.size() + 1;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ frequency-counting.hpp -o demo && ./demo
int main() {
    vector<int> a = {4, 1, 4, 0, 1};
    auto freq = frequencyTable(a);
    auto runs = runLengthEncode(string("AAABBCC"));
    printf("distinct=%zu, mex=%d, runs=%zu\n", freq.size(), mexNonnegative(a), runs.size());
    return 0;
}
#endif
