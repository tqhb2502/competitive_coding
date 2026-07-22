#include <functional>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int boys, girls, possiblePairs;
    cin >> boys >> girls >> possiblePairs;
    vector<vector<int>> graph(boys + 1);
    for (int edge = 0; edge < possiblePairs; ++edge) {
        int boy, girl;
        cin >> boy >> girl;
        graph[boy].push_back(girl);
    }

    vector<int> matchLeft(boys + 1, 0);
    vector<int> matchRight(girls + 1, 0);
    vector<int> visited(girls + 1, 0);

    function<bool(int, int)> augment = [&](int boy, int token) {
        for (const int girl : graph[boy]) {
            if (visited[girl] == token) {
                continue;
            }
            visited[girl] = token;
            if (matchRight[girl] == 0 || augment(matchRight[girl], token)) {
                matchLeft[boy] = girl;
                matchRight[girl] = boy;
                return true;
            }
        }
        return false;
    };

    int matchingSize = 0;
    for (int boy = 1; boy <= boys; ++boy) {
        if (augment(boy, boy)) {
            ++matchingSize;
        }
    }

    cout << matchingSize << '\n';
    for (int boy = 1; boy <= boys; ++boy) {
        if (matchLeft[boy] != 0) {
            cout << boy << ' ' << matchLeft[boy] << '\n';
        }
    }
    return 0;
}
