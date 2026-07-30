#pragma once
#include <bits/stdc++.h>
using namespace std;
// Path Matching — greedy hối tiếc/co cạnh cho min-cost matching theo mọi lực lượng trên đường đi.
// Khi dùng: chọn đúng k cạnh không chung đỉnh trên path; cần đáp án cho nhiều k (CSES Minimum Cost Pairs).
// ĐPT: O((m+k) log m) cho m cạnh và các k=0..maxPairs; bộ nhớ O(m).
// Dùng: auto best = minCostPathMatching({4,1,5}, -1); // best[k] là min cost chọn đúng k cạnh
// Bẫy: edgeCost[i] nối đỉnh i,i+1 (0-based), được phép âm; maxPairs=-1 nghĩa là mọi k;
//       tính nội bộ bằng __int128 nhưng ném overflow_error nếu một đáp án không vừa long long.
// CSES: 3402

// Ý nghĩa: tính chi phí nhỏ nhất khi chọn đúng k cạnh đôi một không kề cho mọi k trong một prefix.
// Tham số: edgeCost = trọng số m cạnh theo thứ tự path; maxPairs = k lớn nhất cần tính,
//          thuộc [0,(m+1)/2], hoặc -1 để tính tới lực lượng matching cực đại.
// Trả về: best kích thước maxPairs+1 với best[k] là min cost chọn đúng k cạnh và best[0]=0.
vector<long long> minCostPathMatching(const vector<long long>& edgeCost, int maxPairs = -1) {
    if (edgeCost.size() > (size_t)INT_MAX)
        throw length_error("minCostPathMatching: quá nhiều cạnh cho chỉ số int");
    int edgeCount = (int)edgeCost.size();
    int possible = edgeCount / 2 + edgeCount % 2;
    if (maxPairs == -1) maxPairs = possible;
    if (maxPairs < 0 || maxPairs > possible)
        throw invalid_argument("minCostPathMatching: maxPairs ngoài miền hợp lệ");

    vector<long long> answer;
    answer.reserve((size_t)maxPairs + 1);
    answer.push_back(0);
    if (maxPairs == 0) return answer;

    struct PathMatchingHeapEntry {
        __int128 cost;
        int edge;
        int version;
        bool operator>(const PathMatchingHeapEntry& other) const {
            if (cost != other.cost) return cost > other.cost;
            if (edge != other.edge) return edge > other.edge;
            return version > other.version;
        }
    };

    vector<__int128> cost(edgeCount);
    vector<int> left(edgeCount), right(edgeCount), version(edgeCount, 0);
    vector<char> alive(edgeCount, true);
    priority_queue<PathMatchingHeapEntry, vector<PathMatchingHeapEntry>,
                   greater<PathMatchingHeapEntry>> heap;
    for (int edge = 0; edge < edgeCount; ++edge) {
        cost[edge] = edgeCost[edge];
        left[edge] = edge - 1;
        right[edge] = edge + 1 < edgeCount ? edge + 1 : -1;
        heap.push({cost[edge], edge, 0});
    }

    __int128 total = 0;
    for (int pairCount = 1; pairCount <= maxPairs; ++pairCount) {
        PathMatchingHeapEntry chosen{};
        while (true) {
            if (heap.empty())
                throw logic_error("minCostPathMatching: heap rỗng trước khi đủ matching");
            chosen = heap.top();
            heap.pop();
            if (alive[chosen.edge] && chosen.version == version[chosen.edge] &&
                chosen.cost == cost[chosen.edge]) break;
        }

        int edge = chosen.edge;
        total += chosen.cost;
        if (total < (__int128)LLONG_MIN || total > (__int128)LLONG_MAX)
            throw overflow_error("minCostPathMatching: đáp án vượt long long");
        answer.push_back((long long)total);

        int l = left[edge], r = right[edge];
        if (l != -1 && r != -1) {
            int outsideLeft = left[l], outsideRight = right[r];
            alive[l] = alive[r] = false;
            cost[edge] = cost[l] + cost[r] - cost[edge];
            ++version[edge];
            left[edge] = outsideLeft;
            right[edge] = outsideRight;
            if (outsideLeft != -1) right[outsideLeft] = edge;
            if (outsideRight != -1) left[outsideRight] = edge;
            heap.push({cost[edge], edge, version[edge]});
        } else {
            // Chọn meta-edge ở biên: nó và người hàng xóm duy nhất không còn tạo
            // được phương án hối tiếc hữu hạn, nên xóa hẳn thay cho sentinel +INF.
            alive[edge] = false;
            if (l != -1) {
                int outsideLeft = left[l];
                alive[l] = false;
                if (outsideLeft != -1) right[outsideLeft] = -1;
            }
            if (r != -1) {
                int outsideRight = right[r];
                alive[r] = false;
                if (outsideRight != -1) left[outsideRight] = -1;
            }
        }
    }
    return answer;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ path-matching.hpp -o demo && ./demo
int main() {
    auto best = minCostPathMatching({4, 1, 5});
    printf("k=0:%lld k=1:%lld k=2:%lld\n", best[0], best[1], best[2]);  // 0, 1, 9
    return 0;
}
#endif
