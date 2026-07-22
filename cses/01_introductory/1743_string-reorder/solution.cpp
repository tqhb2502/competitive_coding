#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string input;
    cin >> input;
    const int length = static_cast<int>(input.size());

    array<int, 26> count{};
    for (char character : input) {
        ++count[character - 'A'];
    }

    int currentMaximum = *max_element(count.begin(), count.end());
    if (2 * currentMaximum > length + 1) {
        cout << "-1\n";
        return 0;
    }

    // frequency[value] is the number of letters with this remaining count.
    vector<int> frequency(currentMaximum + 2, 0);
    for (int value : count) {
        ++frequency[value];
    }

    vector<int> active;
    for (int letter = 0; letter < 26; ++letter) {
        if (count[letter] > 0) {
            active.push_back(letter);
        }
    }

    int maximumLetter = 0;
    for (int letter = 0; letter < 26; ++letter) {
        if (count[letter] == currentMaximum) {
            maximumLetter = letter;
            break;
        }
    }

    string result(length, ' ');
    int previous = -1;
    int remaining = length;

    for (int position = 0; position < length; ++position) {
        int chosen;
        if (2 * currentMaximum > remaining) {
            // Tight state: the unique dominant letter must be placed now.
            if (count[maximumLetter] != currentMaximum) {
                for (int letter : active) {
                    if (count[letter] == currentMaximum) {
                        maximumLetter = letter;
                        break;
                    }
                }
            }
            chosen = maximumLetter;
        } else {
            // Loose state: choose the smallest available letter != previous.
            chosen = active.front();
            if (chosen == previous) {
                chosen = active[1];
            }
        }

        result[position] = static_cast<char>('A' + chosen);

        const int oldCount = count[chosen];
        --count[chosen];
        --frequency[oldCount];
        ++frequency[oldCount - 1];
        if (oldCount == currentMaximum && frequency[currentMaximum] == 0) {
            --currentMaximum;
        }
        if (oldCount == 1) {
            active.erase(find(active.begin(), active.end(), chosen));
        }

        previous = chosen;
        --remaining;
    }

    cout << result << '\n';
    return 0;
}
