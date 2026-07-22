#include <array>
#include <cstddef>
#include <iostream>
#include <string>

using namespace std;

size_t letterIndex(char letter) {
    if (letter == 'A') {
        return 0;
    }
    if (letter == 'C') {
        return 1;
    }
    if (letter == 'G') {
        return 2;
    }
    return 3;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string dna;
    cin >> dna;

    array<bool, 4> seen{};
    int distinct = 0;
    string answer;
    answer.reserve(dna.size() / 4 + 1);

    for (char letter : dna) {
        size_t index = letterIndex(letter);
        if (!seen[index]) {
            seen[index] = true;
            ++distinct;
        }
        if (distinct == 4) {
            answer.push_back(letter);
            seen.fill(false);
            distinct = 0;
        }
    }

    constexpr array<char, 4> LETTERS{'A', 'C', 'G', 'T'};
    for (size_t index = 0; index < 4; ++index) {
        if (!seen[index]) {
            answer.push_back(LETTERS[index]);
            break;
        }
    }

    cout << answer << '\n';
}
