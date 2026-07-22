#include <algorithm>
#include <iostream>
#include <string>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string sequence;
    std::cin >> sequence;

    int best = 0;
    int current = 0;
    char previous = '\0';

    for (char nucleotide : sequence) {
        if (nucleotide == previous) {
            ++current;
        } else {
            previous = nucleotide;
            current = 1;
        }
        best = std::max(best, current);
    }

    std::cout << best << '\n';
    return 0;
}
