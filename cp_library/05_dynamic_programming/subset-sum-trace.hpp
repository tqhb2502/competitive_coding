#pragma once
#include <bits/stdc++.h>
using namespace std;
// Subset Sum Trace — bitset động 0/1 subset-sum, lưu cha cho mỗi tổng lần đầu đạt được.
// Khi dùng: tổng giới hạn khá lớn; cần vừa kiểm tra reachability vừa dựng một tập chỉ số tạo ra tổng.
// ĐPT: dựng O(n*limit/64 + limit), canMake O(1), subsetIndices O(số phần tử truy vết); bộ nhớ O(limit).
// Dùng: SubsetSumTrace ss({3,1,4,2}, 6); auto ids = ss.subsetIndices(6);
// Bẫy: values phải KHÔNG ÂM, mỗi chỉ số dùng tối đa một lần; tổng trong [0,limit]; index kết quả 0-based.
// CSES: 3159
struct SubsetSumTrace {
private:
    int sumLimit = 0;
    vector<uint64_t> reachable;
    vector<int> parentItem;
    vector<int> parentSum;

public:
    // Ý nghĩa: tạo bảng chỉ chứa tổng 0.
    SubsetSumTrace() { build({}, 0); }

    // Ý nghĩa: dựng reachability và vết của mọi tổng không vượt quá limit.
    // Tham số: values = các trọng số không âm; limit = tổng lớn nhất cần lưu (>=0).
    SubsetSumTrace(const vector<int>& values, int limit) { build(values, limit); }

    // Ý nghĩa: lấy tổng lớn nhất cấu trúc đang lưu.
    // Trả về: limit đã truyền cho lần build gần nhất.
    int limit() const { return sumLimit; }

    // Ý nghĩa: dựng lại bảng cho một dãy và giới hạn mới.
    // Tham số: values = các trọng số không âm; limit = tổng lớn nhất cần lưu (>=0).
    void build(const vector<int>& values, int limit) {
        if (limit < 0) throw invalid_argument("SubsetSumTrace::build: limit âm");
        if (values.size() > (size_t)INT_MAX)
            throw length_error("SubsetSumTrace::build: quá nhiều phần tử cho chỉ số int");
        for (int value : values)
            if (value < 0) throw invalid_argument("SubsetSumTrace::build: giá trị âm");

        sumLimit = limit;
        int wordCount = limit / 64 + 1;
        reachable.assign(wordCount, 0);
        reachable[0] = 1;
        parentItem.assign((size_t)limit + 1, -1);
        parentSum.assign((size_t)limit + 1, -1);

        for (int item = 0; item < (int)values.size(); ++item) {
            int shift = values[item];
            if (shift == 0 || shift > limit) continue;
            int wordShift = shift / 64;
            int bitShift = shift % 64;
            for (int destination = wordCount - 1; destination >= wordShift; --destination) {
                int source = destination - wordShift;
                uint64_t shifted = reachable[source] << bitShift;
                if (bitShift != 0 && source > 0)
                    shifted |= reachable[source - 1] >> (64 - bitShift);
                if (destination == wordCount - 1 && limit % 64 != 63)
                    shifted &= (uint64_t{1} << (limit % 64 + 1)) - 1;

                uint64_t newSums = shifted & ~reachable[destination];
                while (newSums != 0) {
                    int bit = __builtin_ctzll(newSums);
                    int sum = destination * 64 + bit;
                    parentItem[sum] = item;
                    parentSum[sum] = sum - shift;
                    newSums &= newSums - 1;
                }
                reachable[destination] |= shifted;
            }
        }
    }

    // Ý nghĩa: kiểm tra một tổng có tạo được bằng tập con hay không.
    // Tham số: sum = tổng trong đoạn đóng [0,sumLimit].
    // Trả về: true nếu có tập con các chỉ số tạo ra sum; ném out_of_range nếu sum ngoài giới hạn.
    bool canMake(int sum) const {
        checkSum(sum);
        return (reachable[sum / 64] >> (sum % 64)) & uint64_t{1};
    }

    // Ý nghĩa: truy vết một tập con tạo ra tổng yêu cầu.
    // Tham số: sum = tổng trong đoạn đóng [0,sumLimit].
    // Trả về: optional chứa các chỉ số 0-based, tăng dần và không lặp; nullopt nếu tổng không đạt được.
    optional<vector<int>> subsetIndices(int sum) const {
        if (!canMake(sum)) return nullopt;
        vector<int> indices;
        while (sum != 0) {
            int item = parentItem[sum];
            if (item < 0 || parentSum[sum] < 0 || parentSum[sum] >= sum)
                throw logic_error("SubsetSumTrace: invariant truy vết bị hỏng");
            indices.push_back(item);
            sum = parentSum[sum];
        }
        reverse(indices.begin(), indices.end());
        return indices;
    }

private:
    // Ý nghĩa: kiểm tra tổng truy vấn thuộc giới hạn, ném out_of_range nếu sai.
    // Tham số: sum = tổng cần kiểm tra.
    void checkSum(int sum) const {
        if (sum < 0 || sum > sumLimit)
            throw out_of_range("SubsetSumTrace: tổng ngoài [0,sumLimit]");
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ subset-sum-trace.hpp -o demo && ./demo
int main() {
    vector<int> values = {3, 1, 4, 2};
    SubsetSumTrace trace(values, 6);
    auto indices = trace.subsetIndices(6);
    int sum = 0;
    printf("tao duoc 6=%d; chi so:", (int)indices.has_value());
    for (int index : *indices) {
        printf(" %d", index);
        sum += values[index];
    }
    printf("; tong=%d\n", sum);  // một đáp án hợp lệ, tổng = 6
    return 0;
}
#endif
