#pragma once
#include <bits/stdc++.h>
using namespace std;
// Probability & Expectation — phân phối tổng xúc xắc, kỳ vọng qua CDF/indicator và random walk độc lập.
// Khi dùng: tổng biến ngẫu nhiên rời rạc; E[max]; E[số inversion]; E[số state rỗng sau các Markov walk độc lập].
// ĐPT: dice O(dice^2*sides^2); E[max] O(max); inversion O(n^2); walkers O(R*steps*E); bộ nhớ tùy hàm.
// Dùng: auto p=diceSumDistribution(n); diceSumRangeProbability(n,a,b); expectedMaximumDiscreteUniform(n,k);
// Bẫy: xác suất dùng long double; transition[u] phải có tổng xác suất 1; các walker phải độc lập; chỉ làm tròn khi in.
// CSES: 1725 1726 1727 1728
// Ý nghĩa: tích chập hai phân phối rời rạc độc lập, chỉ số vector chính là giá trị biến.
// Tham số: left, right = xác suất của hai biến nhận từng giá trị nguyên không âm.
// Trả về: phân phối của tổng hai biến, cỡ left.size()+right.size()-1; rỗng nếu một đầu vào rỗng.
vector<long double> convolveProbabilityDistributions(
        const vector<long double>& left, const vector<long double>& right) {
    if (left.empty() || right.empty()) return {};
    vector<long double> result(left.size() + right.size() - 1, 0);
    for (int i = 0; i < (int)left.size(); i++)
        if (left[i] != 0)
            for (int j = 0; j < (int)right.size(); j++)
                result[i + j] += left[i] * right[j];
    return result;
}

// Ý nghĩa: tính phân phối tổng của các xúc xắc công bằng độc lập.
// Tham số: dice = số xúc xắc không âm; sides = số mặt dương, mỗi mặt mang giá trị 1..sides.
// Trả về: result[sum] = P(tổng bằng sum), vector cỡ dice*sides+1.
vector<long double> diceSumDistribution(int dice, int sides = 6) {
    if (dice < 0 || sides <= 0) throw invalid_argument("diceSumDistribution: invalid dice or sides");
    vector<long double> distribution(1, 1);
    for (int used = 0; used < dice; used++) {
        vector<long double> next((used + 1) * sides + 1, 0);
        for (int sum = used; sum <= used * sides; sum++) {
            if (distribution[sum] == 0) continue;
            for (int face = 1; face <= sides; face++)
                next[sum + face] += distribution[sum] / sides;
        }
        distribution.swap(next);
    }
    return distribution;
}

// Ý nghĩa: tính xác suất tổng xúc xắc nằm trong đoạn đóng [low, high].
// Tham số: dice = số xúc xắc; low, high = biên tổng; sides = số mặt.
// Trả về: tổng xác suất trong đoạn, bằng 0 nếu đoạn không giao miền giá trị.
long double diceSumRangeProbability(int dice, int low, int high, int sides = 6) {
    vector<long double> distribution = diceSumDistribution(dice, sides);
    low = max(low, 0);
    high = min(high, (int)distribution.size() - 1);
    if (low > high) return 0;
    long double answer = 0;
    for (int sum = low; sum <= high; sum++) answer += distribution[sum];
    return answer;
}

// Ý nghĩa: tính kỳ vọng của biến nguyên không âm từ phân phối xác suất.
// Tham số: distribution = distribution[value] là xác suất X=value.
// Trả về: E[X] = tổng value*P(X=value).
long double expectationFromDistribution(const vector<long double>& distribution) {
    long double answer = 0;
    for (int value = 0; value < (int)distribution.size(); value++)
        answer += value * distribution[value];
    return answer;
}

// Ý nghĩa: tính E[max] của draws biến độc lập, đều trên các số nguyên 1..maximum_value bằng tổng xác suất đuôi.
// Tham số: draws = số biến dương; maximum_value = cận trên dương.
// Trả về: kỳ vọng của giá trị lớn nhất.
long double expectedMaximumDiscreteUniform(int draws, int maximum_value) {
    if (draws <= 0 || maximum_value <= 0)
        throw invalid_argument("expectedMaximumDiscreteUniform: arguments must be positive");
    long double answer = maximum_value;
    for (int value = 1; value < maximum_value; value++)
        answer -= pow((long double)value / maximum_value, draws);
    return answer;
}

// Ý nghĩa: tính P(X > Y) khi X đều trên [1,left_max], Y đều trên [1,right_max] và độc lập.
// Tham số: left_max, right_max = cận trên dương của hai phân phối đều.
// Trả về: xác suất X > Y.
long double probabilityUniformGreater(long long left_max, long long right_max) {
    if (left_max <= 0 || right_max <= 0)
        throw invalid_argument("probabilityUniformGreater: bounds must be positive");
    long long m = min(right_max, left_max - 1);
    long double favorable = (long double)m * left_max - (long double)m * (m + 1) / 2;
    return favorable / ((long double)left_max * right_max);
}

// Ý nghĩa: tính kỳ vọng số inversion bằng tính tuyến tính của kỳ vọng trên mọi cặp chỉ số.
// Tham số: upper_bound[i] = phần tử i được chọn độc lập và đều từ [1, upper_bound[i]].
// Trả về: E[số cặp i<j với X_i>X_j].
long double expectedUniformInversions(const vector<long long>& upper_bound) {
    long double answer = 0;
    for (int i = 0; i < (int)upper_bound.size(); i++)
        for (int j = i + 1; j < (int)upper_bound.size(); j++)
            answer += probabilityUniformGreater(upper_bound[i], upper_bound[j]);
    return answer;
}

// Ý nghĩa: tính phân phối đích của mỗi walker sau một số bước Markov; mỗi walker bắt đầu chắc chắn tại starts[r].
// Tham số: transition[u] = các cặp (v,P(u->v)); steps = số bước; starts = state bắt đầu của từng walker.
// Trả về: result[r][v] = xác suất walker r kết thúc tại v sau đúng steps bước.
vector<vector<long double>> independentWalkDistributions(
        const vector<vector<pair<int, long double>>>& transition,
        int steps, const vector<int>& starts) {
    int states = (int)transition.size();
    if (steps < 0) throw invalid_argument("independentWalkDistributions: negative steps");
    for (int start : starts)
        if (start < 0 || start >= states) throw invalid_argument("independentWalkDistributions: invalid start");
    for (const auto& edges : transition)
        for (auto [to, probability] : edges)
            if (to < 0 || to >= states || probability < 0)
                throw invalid_argument("independentWalkDistributions: invalid transition");

    vector<vector<long double>> distribution(starts.size(), vector<long double>(states, 0));
    for (int walker = 0; walker < (int)starts.size(); walker++)
        distribution[walker][starts[walker]] = 1;
    for (int step = 0; step < steps; step++) {
        vector<vector<long double>> next(starts.size(), vector<long double>(states, 0));
        for (int walker = 0; walker < (int)starts.size(); walker++) {
            for (int from = 0; from < states; from++) {
                if (distribution[walker][from] == 0) continue;
                for (auto [to, probability] : transition[from])
                    next[walker][to] += distribution[walker][from] * probability;
            }
        }
        distribution.swap(next);
    }
    return distribution;
}

// Ý nghĩa: tính kỳ vọng số state không chứa walker nào sau các random walk độc lập.
// Tham số: transition = chuỗi Markov; steps = số bước; starts = vị trí đầu của từng walker (cho phép trùng).
// Trả về: tổng theo state v của tích_r (1-P(walker r kết thúc ở v)).
long double expectedEmptyStatesAfterIndependentWalks(
        const vector<vector<pair<int, long double>>>& transition,
        int steps, const vector<int>& starts) {
    vector<vector<long double>> distribution =
        independentWalkDistributions(transition, steps, starts);
    long double answer = 0;
    for (int state = 0; state < (int)transition.size(); state++) {
        long double empty_probability = 1;
        for (const auto& walker : distribution)
            empty_probability *= max((long double)0, (long double)1 - walker[state]);
        answer += empty_probability;
    }
    return answer;
}

// Ý nghĩa: bản tiện dụng khi ban đầu mỗi state có đúng một walker.
// Tham số: transition = chuỗi Markov; steps = số bước.
// Trả về: kỳ vọng số state rỗng sau steps bước.
long double expectedEmptyStatesAfterIndependentWalks(
        const vector<vector<pair<int, long double>>>& transition, int steps) {
    vector<int> starts(transition.size());
    iota(starts.begin(), starts.end(), 0);
    return expectedEmptyStatesAfterIndependentWalks(transition, steps, starts);
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ probability-expectation.hpp -o demo && ./demo
int main() {
    printf("P(2d6 có tổng 7)=%.9Lf\n", diceSumRangeProbability(2, 7, 7));       // 1/6
    printf("E[max của 2 số 1..3]=%.9Lf\n", expectedMaximumDiscreteUniform(2, 3)); // 22/9
    printf("E[inversion] [5,2,7]=%.9Lf\n", expectedUniformInversions({5, 2, 7})); // 1.057142857
    return 0;
}
#endif
