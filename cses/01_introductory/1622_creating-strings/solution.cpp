#include <algorithm>
#include <iostream>
#include <set>
#include <string>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;
    sort(s.begin(), s.end());

    set<string> distinct_permutations;
    do {
        distinct_permutations.insert(s);
    } while (next_permutation(s.begin(), s.end()));

    cout << distinct_permutations.size() << '\n';
    for (const string& permutation : distinct_permutations) {
        cout << permutation << '\n';
    }
    return 0;
}
