#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int programmers;
    int artists;
    int applicantCount;
    cin >> programmers >> artists >> applicantCount;

    vector<pair<long long, long long>> applicants(applicantCount);
    for (auto& [programmingSkill, artisticSkill] : applicants) {
        cin >> programmingSkill >> artisticSkill;
    }
    sort(applicants.begin(), applicants.end(),
         [](const auto& first, const auto& second) {
             return first.first - first.second >
                    second.first - second.second;
         });

    const long long impossible = numeric_limits<long long>::min() / 4;
    vector<long long> prefix(applicantCount + 1, impossible);
    priority_queue<long long, vector<long long>, greater<long long>> heap;
    long long sum = 0;
    for (int index = 0; index <= applicantCount; ++index) {
        if (static_cast<int>(heap.size()) == programmers) {
            prefix[index] = sum;
        }
        if (index == applicantCount) {
            break;
        }
        heap.push(applicants[index].first);
        sum += applicants[index].first;
        if (static_cast<int>(heap.size()) > programmers) {
            sum -= heap.top();
            heap.pop();
        }
    }

    vector<long long> suffix(applicantCount + 1, impossible);
    heap = priority_queue<long long, vector<long long>, greater<long long>>();
    sum = 0;
    for (int index = applicantCount; index >= 0; --index) {
        if (static_cast<int>(heap.size()) == artists) {
            suffix[index] = sum;
        }
        if (index == 0) {
            break;
        }
        heap.push(applicants[index - 1].second);
        sum += applicants[index - 1].second;
        if (static_cast<int>(heap.size()) > artists) {
            sum -= heap.top();
            heap.pop();
        }
    }

    long long answer = 0;
    for (int split = 0; split <= applicantCount; ++split) {
        if (prefix[split] != impossible && suffix[split] != impossible) {
            answer = max(answer, prefix[split] + suffix[split]);
        }
    }
    cout << answer << '\n';
}
