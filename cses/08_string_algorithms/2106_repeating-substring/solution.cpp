#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string text;
    if (!(std::cin >> text)) {
        return 0;
    }

    const int length = static_cast<int>(text.size());
    if (length == 1) {
        std::cout << -1 << '\n';
        return 0;
    }

    std::vector<int> suffix_array(length);
    std::iota(suffix_array.begin(), suffix_array.end(), 0);
    std::vector<int> rank(length);
    for (int index = 0; index < length; ++index) {
        rank[index] = text[index] - 'a';
    }

    std::vector<int> next_rank(length);
    for (int step = 1;; step *= 2) {
        const auto less_suffix = [&](const int left, const int right) {
            if (rank[left] != rank[right]) {
                return rank[left] < rank[right];
            }
            const int left_second = left + step < length ? rank[left + step] : -1;
            const int right_second = right + step < length ? rank[right + step] : -1;
            return left_second < right_second;
        };
        std::sort(suffix_array.begin(), suffix_array.end(), less_suffix);

        next_rank[suffix_array[0]] = 0;
        for (int index = 1; index < length; ++index) {
            const int previous = suffix_array[index - 1];
            const int current = suffix_array[index];
            const int previous_second = previous + step < length ? rank[previous + step] : -1;
            const int current_second = current + step < length ? rank[current + step] : -1;
            const bool different = rank[previous] != rank[current] || previous_second != current_second;
            next_rank[current] = next_rank[previous] + (different ? 1 : 0);
        }
        rank.swap(next_rank);

        if (rank[suffix_array.back()] == length - 1) {
            break;
        }
    }

    int best_length = 0;
    int best_position = 0;
    int common = 0;
    for (int position = 0; position < length; ++position) {
        const int order = rank[position];
        if (order == 0) {
            common = 0;
            continue;
        }
        const int previous_suffix = suffix_array[order - 1];
        while (position + common < length && previous_suffix + common < length &&
               text[position + common] == text[previous_suffix + common]) {
            ++common;
        }
        if (common > best_length) {
            best_length = common;
            best_position = position;
        }
        if (common > 0) {
            --common;
        }
    }

    if (best_length == 0) {
        std::cout << -1 << '\n';
    } else {
        std::cout << text.substr(best_position, best_length) << '\n';
    }

    return 0;
}
