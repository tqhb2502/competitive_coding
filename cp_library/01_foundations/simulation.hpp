#pragma once
#include <bits/stdc++.h>
using namespace std;
// Simulation — chạy chuyển trạng thái từng bước, có hook quan sát, giới hạn bước và dò chu trình Floyd.
// Khi dùng: đề mô tả trực tiếp trạng thái kế tiếp; cần mô phỏng an toàn hoặc tìm preperiod/period của dãy trạng thái.
// ĐPT: simulateUntil O(số bước), O(1) phụ; floydCycleInfo O(mu + lambda), O(1) phụ.
// Dùng: simulateUntil(x, next, done, visit, limit); auto [mu, lambda] = floydCycleInfo(x, next);
// Bẫy: step phải xác định; limit < 0 nghĩa là không giới hạn; Floyd chỉ dùng khi dãy cuối cùng đi vào chu trình.
// CSES: 1068 1637 2174
// Ý nghĩa: thăm trạng thái đầu rồi mô phỏng cho tới khi done(state) đúng hoặc chạm giới hạn.
// Tham số: state = trạng thái đầu; step = hàm tạo trạng thái kế; done = điều kiện dừng; visit = hook gọi trước mỗi bước; limit = số bước tối đa (-1 = vô hạn).
// Trả về: số lần gọi step để tới trạng thái dừng; -1 nếu chạm limit trước khi done.
template <class State, class Step, class Done, class Visit>
long long simulateUntil(State state, Step step, Done done, Visit visit, long long limit = -1) {
    long long steps = 0;
    while (true) {
        visit(state);
        if (done(state)) return steps;
        if (limit >= 0 && steps == limit) return -1;
        state = step(state);
        ++steps;
    }
}

// Ý nghĩa: tìm phần không tuần hoàn mu và chu kỳ lambda của dãy x, f(x), f(f(x)), ... bằng Floyd.
// Tham số: start = trạng thái đầu; step = hàm chuyển xác định, trạng thái phải so sánh được bằng ==.
// Trả về: {mu, lambda}; chu trình bắt đầu sau mu bước và dài lambda.
template <class State, class Step>
pair<long long, long long> floydCycleInfo(const State& start, Step step) {
    State slow = step(start), fast = step(step(start));
    while (!(slow == fast)) {
        slow = step(slow);
        fast = step(step(fast));
    }
    long long mu = 0;
    slow = start;
    while (!(slow == fast)) {
        slow = step(slow);
        fast = step(fast);
        ++mu;
    }
    long long lambda = 1;
    fast = step(slow);
    while (!(slow == fast)) {
        fast = step(fast);
        ++lambda;
    }
    return {mu, lambda};
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ simulation.hpp -o demo && ./demo
int main() {
    vector<long long> seq;
    long long steps = simulateUntil(13LL,
        [](long long x) { return x & 1 ? 3 * x + 1 : x / 2; },
        [](long long x) { return x == 1; },
        [&](long long x) { seq.push_back(x); }, 100);
    auto cyc = floydCycleInfo(0, [](int x) { return (x * x + 1) % 10; });
    printf("Collatz(13): %lld bước, %zu trạng thái; cycle: mu=%lld lambda=%lld\n",
           steps, seq.size(), cyc.first, cyc.second);
    return 0;
}
#endif
