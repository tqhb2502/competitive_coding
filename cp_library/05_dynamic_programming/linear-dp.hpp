#pragma once
#include <bits/stdc++.h>
using namespace std;
// Linear DP — runner hai hàng rolling-state và các mẫu quét tuyến tính: Array Description, Removing Digits, Counting Towers, Coding Company.
// Khi dùng: trạng thái lớp i chỉ phụ thuộc lớp i-1; cần giảm O(số_bước*width) bộ nhớ xuống O(width); DP theo tiền tố đã sort.
// ĐPT: runner O(steps*(width + transition)), O(width); Array Description O(nm), O(m); Removing Digits O(n log10 n), O(1).
//      Counting Towers O(H), O(H) do output; Coding Company O(n^2*x + n log n), O(n*x).
// Dùng: auto last = runRollingLinearDp<long long>(steps, initial, [&](int i, const auto& prev, auto& next) { ... });
//        countArrayDescriptions(x,m); minimumRemovingDigitsSteps(n); countTwoColumnTowerWays(H); countBoundedTeamPartitions(skill,x);
// Bẫy: callback phải GHI CỘNG vào next đã được reset về T{} và không được resize; step là 0-based của PHÉP CHUYỂN, initial là lớp trước step 0.
//       Rolling state chỉ hợp lệ khi lớp mới không cần lớp cũ hơn; Coding Company phải sort skill trước khi cộng open*(a[i]-a[i-1]).
// CSES: 1633 1637 1665 1746 2229 2413

// Ý nghĩa: chạy một DP tuyến tính bằng đúng hai vector trạng thái cùng width, tự reset hàng next trước mỗi bước.
// Tham số: transitions = số lần chuyển không âm; initial = lớp ban đầu; transition(step,prev,next) ghi lớp kế, next ban đầu toàn T{}.
// Trả về: trạng thái sau transitions lần chuyển; ném invalid_argument nếu callback thay đổi kích thước next.
template <class T, class Transition>
vector<T> runRollingLinearDp(int transitions, vector<T> initial, Transition&& transition) {
    if (transitions < 0) throw invalid_argument("runRollingLinearDp: transitions âm");
    const size_t width = initial.size();
    vector<T> next(width);
    for (int step = 0; step < transitions; ++step) {
        fill(next.begin(), next.end(), T{});
        std::invoke(transition, step, std::as_const(initial), next);
        if (next.size() != width)
            throw invalid_argument("runRollingLinearDp: transition không được resize next");
        initial.swap(next);
    }
    return initial;
}

// Ý nghĩa: đếm cách điền các số 0 (ẩn) sao cho mọi giá trị thuộc [1,maxValue] và hai phần tử kề chênh không quá 1.
// Tham số: description = mảng mô tả (0 là ẩn, số khác là cố định); maxValue = m >= 1; mod = modulo dương.
// Trả về: số mảng hợp lệ modulo mod; mảng rỗng có đúng một cách.
inline long long countArrayDescriptions(const vector<int>& description, int maxValue,
                                        long long mod = 1000000007LL) {
    if (maxValue <= 0) throw invalid_argument("countArrayDescriptions: maxValue phải dương");
    if (mod <= 0) throw invalid_argument("countArrayDescriptions: mod phải dương");
    for (int value : description) {
        if (value < 0 || value > maxValue)
            throw invalid_argument("countArrayDescriptions: giá trị ngoài [0,maxValue]");
    }
    if (description.empty()) return 1 % mod;

    vector<long long> initial(maxValue + 2, 0);  // hai ô padding 0 và maxValue+1 luôn bằng 0.
    if (description[0] == 0) {
        for (int value = 1; value <= maxValue; ++value) initial[value] = 1 % mod;
    } else {
        initial[description[0]] = 1 % mod;
    }

    auto last = runRollingLinearDp<long long>(
        (int)description.size() - 1, std::move(initial),
        [&](int step, const vector<long long>& previous, vector<long long>& next) {
            int fixed = description[step + 1];
            int low = fixed == 0 ? 1 : fixed;
            int high = fixed == 0 ? maxValue : fixed;
            for (int value = low; value <= high; ++value) {
                next[value] = (long long)(((__int128)previous[value - 1] +
                                           previous[value] + previous[value + 1]) % mod);
            }
        });
    __int128 answer = 0;
    for (int value = 1; value <= maxValue; ++value) answer += last[value];
    return (long long)(answer % mod);
}

// Ý nghĩa: tính số bước ít nhất để đưa n về 0 khi mỗi bước trừ một chữ số khác 0 của số hiện tại.
// Tham số: n = số nguyên không âm.
// Trả về: số bước tối thiểu; dùng vòng tròn 10 ô vì dp[i] chỉ đọc dp[i-d] với 1 <= d <= 9.
inline int minimumRemovingDigitsSteps(int n) {
    if (n < 0) throw invalid_argument("minimumRemovingDigitsSteps: n âm");
    const int INF = numeric_limits<int>::max() / 4;
    array<int, 10> rolling;
    rolling.fill(INF);
    rolling[0] = 0;
    for (int value = 1; value <= n; ++value) {
        int best = INF;
        for (int digits = value; digits > 0; digits /= 10) {
            int digit = digits % 10;
            if (digit != 0) best = min(best, rolling[(value - digit) % 10] + 1);
        }
        rolling[value % 10] = best;
    }
    return rolling[n % 10];
}

// Ý nghĩa: tiền xử lý số cách dựng tháp rộng 2 cho mọi chiều cao 1..maxHeight bằng DP hai trạng thái connected/split.
// Tham số: maxHeight = chiều cao lớn nhất không âm; mod = modulo dương.
// Trả về: answer kích thước maxHeight+1, answer[h] là đáp án CSES cho h>=1; quy ước answer[0]=1 cho tháp rỗng.
inline vector<long long> countTwoColumnTowerWays(int maxHeight, long long mod = 1000000007LL) {
    if (maxHeight < 0) throw invalid_argument("countTwoColumnTowerWays: maxHeight âm");
    if (mod <= 0) throw invalid_argument("countTwoColumnTowerWays: mod phải dương");
    vector<long long> answer(maxHeight + 1, 0);
    answer[0] = 1 % mod;
    if (maxHeight == 0) return answer;
    long long connected = 1 % mod, split = 1 % mod;
    answer[1] = (long long)(((__int128)connected + split) % mod);
    for (int height = 2; height <= maxHeight; ++height) {
        long long nextConnected = (long long)((2 * (__int128)connected + split) % mod);
        long long nextSplit = (long long)(((__int128)connected + 4 * (__int128)split) % mod);
        connected = nextConnected;
        split = nextSplit;
        answer[height] = (long long)(((__int128)connected + split) % mod);
    }
    return answer;
}

// Ý nghĩa: đếm phân hoạch các lập trình viên thành đội không rỗng sao cho tổng (max skill - min skill) của các đội không quá maxPenalty.
// Tham số: skills = kỹ năng của các cá nhân phân biệt (nhận bản sao để sort); maxPenalty = x không âm; mod = modulo dương.
// Trả về: số phân hoạch hợp lệ modulo mod; tập rỗng có một phân hoạch rỗng.
inline long long countBoundedTeamPartitions(vector<int> skills, int maxPenalty,
                                            long long mod = 1000000007LL) {
    if (maxPenalty < 0) throw invalid_argument("countBoundedTeamPartitions: maxPenalty âm");
    if (mod <= 0) throw invalid_argument("countBoundedTeamPartitions: mod phải dương");
    sort(skills.begin(), skills.end());
    int n = (int)skills.size();
    vector<vector<long long>> current(n + 1, vector<long long>(maxPenalty + 1, 0));
    vector<vector<long long>> next(n + 1, vector<long long>(maxPenalty + 1, 0));
    current[0][0] = 1 % mod;

    auto addWays = [&](long long& destination, long long ways, int choices) {
        destination = (long long)(((__int128)destination +
                                   (__int128)ways * choices) % mod);
    };
    for (int i = 0; i < n; ++i) {
        for (auto& row : next) fill(row.begin(), row.end(), 0);
        long long gap = i == 0 ? 0LL : (long long)skills[i] - skills[i - 1];
        for (int open = 0; open <= i; ++open) {
            long long added = gap * open;
            if (added > maxPenalty) continue;
            for (int cost = 0; cost + added <= maxPenalty; ++cost) {
                long long ways = current[open][cost];
                if (ways == 0) continue;
                int shiftedCost = (int)(cost + added);
                // Đội singleton hoặc thêm vào một đội mà vẫn để mở: tổng cộng open+1 lựa chọn.
                addWays(next[open][shiftedCost], ways, open + 1);
                // Mở một đội mới tại người hiện tại.
                addWays(next[open + 1][shiftedCost], ways, 1);
                // Đóng một trong các đội đang mở tại người hiện tại.
                if (open > 0) addWays(next[open - 1][shiftedCost], ways, open);
            }
        }
        current.swap(next);
    }

    __int128 answer = 0;
    for (int cost = 0; cost <= maxPenalty; ++cost) answer += current[0][cost];
    return (long long)(answer % mod);
}

#ifdef CP_DEMO  // g++ -std=c++17 -O2 -DCP_DEMO -x c++ linear-dp.hpp -o demo && ./demo
int main() {
    printf("array=%lld; remove27=%d\n",
           countArrayDescriptions({2, 0, 2}, 3), minimumRemovingDigitsSteps(27));
    // array=3; remove27=5

    auto towers = countTwoColumnTowerWays(6);
    printf("towers: h2=%lld h6=%lld; teams=%lld\n",
           towers[2], towers[6], countBoundedTeamPartitions({1, 3, 3}, 2));
    // towers: h2=8 h6=2864; teams=5
    return 0;
}
#endif
