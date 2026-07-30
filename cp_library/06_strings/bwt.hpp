#pragma once
#include <bits/stdc++.h>
using namespace std;
// Burrows-Wheeler Transform (BWT) — biến đổi các phép quay và khôi phục tuyến tính bằng LF mapping.
// Khi dùng: đề cho cột cuối của các rotation đã sắp xếp; nén chuỗi/FM-index; cần inverse BWT có sentinel duy nhất.
// ĐPT: transform O(n log n), inverse O(n + 256); bộ nhớ O(n). Thứ tự ký tự theo unsigned byte.
// Dùng: string last = burrowsWheelerTransform(s); string original = inverseBurrowsWheeler(last);
// Bẫy: sentinel phải xuất hiện đúng một lần và không có sẵn trong s; mặc định '#'; kết quả transform luôn chứa sentinel.
// CSES: 1113
// Ý nghĩa: tạo BWT bằng sắp xếp các phép quay theo thuật toán doubling.
// Tham số: s = xâu gốc không chứa sentinel; sentinel = byte canh gác duy nhất được nối vào cuối.
// Trả về: cột cuối của các rotation của s + sentinel, sắp theo thứ tự từ điển unsigned byte.
string burrowsWheelerTransform(const string& s, unsigned char sentinel = '#') {
    for (unsigned char ch : s)
        if (ch == sentinel) throw invalid_argument("burrowsWheelerTransform: sentinel occurs in input");

    string text = s;
    text.push_back((char)sentinel);
    int n = (int)text.size();
    vector<int> order(n), cls(n), next_order(n), next_cls(n);
    vector<int> count(max(256, n), 0);
    for (unsigned char ch : text) count[ch]++;
    for (int c = 1; c < 256; c++) count[c] += count[c - 1];
    for (int i = n - 1; i >= 0; i--) order[--count[(unsigned char)text[i]]] = i;
    int classes = 1;
    cls[order[0]] = 0;
    for (int i = 1; i < n; i++) {
        if ((unsigned char)text[order[i]] != (unsigned char)text[order[i - 1]]) classes++;
        cls[order[i]] = classes - 1;
    }

    for (int length = 1; length < n;) {
        fill(count.begin(), count.begin() + classes, 0);
        for (int i = 0; i < n; i++) {
            next_order[i] = order[i] - length;
            if (next_order[i] < 0) next_order[i] += n;
            count[cls[next_order[i]]]++;
        }
        for (int c = 1; c < classes; c++) count[c] += count[c - 1];
        for (int i = n - 1; i >= 0; i--) {
            int start = next_order[i];
            order[--count[cls[start]]] = start;
        }

        next_cls[order[0]] = 0;
        int next_classes = 1;
        for (int i = 1; i < n; i++) {
            pair<int, int> now = {cls[order[i]], cls[(order[i] + length) % n]};
            pair<int, int> before = {cls[order[i - 1]], cls[(order[i - 1] + length) % n]};
            if (now != before) next_classes++;
            next_cls[order[i]] = next_classes - 1;
        }
        cls.swap(next_cls);
        classes = next_classes;
        if (length > n / 2) break;
        length *= 2;
    }

    string last(n, '\0');
    for (int row = 0; row < n; row++) last[row] = text[(order[row] + n - 1) % n];
    return last;
}

// Ý nghĩa: khôi phục xâu gốc từ cột cuối BWT bằng LF[i] = C[L[i]] + rank(i).
// Tham số: last = cột cuối có đúng một sentinel; sentinel = byte canh gác đã dùng khi transform.
// Trả về: xâu gốc (đã bỏ sentinel).
string inverseBurrowsWheeler(const string& last, unsigned char sentinel = '#') {
    if (last.empty()) throw invalid_argument("inverseBurrowsWheeler: missing sentinel");
    array<int, 256> frequency{}, start{}, seen{};
    int sentinel_count = 0;
    for (unsigned char ch : last) {
        frequency[ch]++;
        sentinel_count += ch == sentinel;
    }
    if (sentinel_count != 1) throw invalid_argument("inverseBurrowsWheeler: sentinel must be unique");
    for (int c = 1; c < 256; c++) start[c] = start[c - 1] + frequency[c - 1];

    int n = (int)last.size();
    vector<int> lf(n);
    for (int i = 0; i < n; i++) {
        unsigned char ch = (unsigned char)last[i];
        lf[i] = start[ch] + seen[ch]++;
    }

    string restored(n, '\0');
    vector<char> visited(n, false);
    int row = start[sentinel];  // hàng mà cột đầu là sentinel: rotation sentinel + s
    for (int i = n - 1; i >= 0; i--) {
        if (visited[row]) throw invalid_argument("inverseBurrowsWheeler: LF mapping is not one cycle");
        visited[row] = true;
        restored[i] = last[row];
        row = lf[row];
    }
    if (row != start[sentinel] || (unsigned char)restored[0] != sentinel)
        throw invalid_argument("inverseBurrowsWheeler: invalid LF cycle");
    return restored.substr(1);
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ bwt.hpp -o demo && ./demo
int main() {
    string last = burrowsWheelerTransform("banana");
    printf("BWT=%s; inverse=%s\n", last.c_str(), inverseBurrowsWheeler(last).c_str());  // annb#aa; banana
    return 0;
}
#endif
