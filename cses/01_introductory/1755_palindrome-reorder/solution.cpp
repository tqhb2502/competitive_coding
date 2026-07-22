#include <array>
#include <iostream>
#include <string>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string input;
    cin >> input;

    array<int, 26> count{};
    for (char character : input) {
        ++count[character - 'A'];
    }

    int oddLetter = -1;
    int oddCount = 0;
    for (int letter = 0; letter < 26; ++letter) {
        if (count[letter] % 2 == 1) {
            oddLetter = letter;
            ++oddCount;
        }
    }

    if (oddCount > 1) {
        cout << "NO SOLUTION\n";
        return 0;
    }

    string leftHalf;
    leftHalf.reserve(input.size() / 2);
    for (int letter = 0; letter < 26; ++letter) {
        leftHalf.append(count[letter] / 2,
                        static_cast<char>('A' + letter));
    }

    string rightHalf(leftHalf.rbegin(), leftHalf.rend());
    cout << leftHalf;
    if (oddLetter != -1) {
        cout << static_cast<char>('A' + oddLetter);
    }
    cout << rightHalf << '\n';
    return 0;
}
