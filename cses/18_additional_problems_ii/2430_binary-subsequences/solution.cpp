#include <algorithm>
#include <iostream>
#include <limits>
#include <string>

using namespace std;

int subtractionSteps(int first, int second) {
    int steps = 0;
    while (first != second) {
        if (first > second) {
            const int count = (first - 1) / second;
            first -= count * second;
            steps += count;
        } else {
            const int count = (second - 1) / first;
            second -= count * first;
            steps += count;
        }
    }
    return first == 1 ? steps : numeric_limits<int>::max();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int subsequences;
    cin >> subsequences;
    const int sum = subsequences + 2;
    int bestFirst = 1;
    int bestSteps = numeric_limits<int>::max();
    for (int first = 1; first <= sum / 2; ++first) {
        const int steps = subtractionSteps(first, sum - first);
        if (steps < bestSteps) {
            bestSteps = steps;
            bestFirst = first;
        }
    }

    int first = bestFirst;
    int second = sum - first;
    string answer;
    answer.reserve(static_cast<size_t>(bestSteps));
    while (first != second) {
        if (first > second) {
            first -= second;
            answer.push_back('0');
        } else {
            second -= first;
            answer.push_back('1');
        }
    }
    reverse(answer.begin(), answer.end());
    cout << answer << '\n';
}
