#include <functional>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int boys, girls, possiblePairs;
    cin >> boys >> girls >> possiblePairs;

    // Danh sách kề: với mỗi nam, các nữ mà nam đó có thể nhảy cùng.
    vector<vector<int>> graph(boys + 1);
    for (int edge = 0; edge < possiblePairs; ++edge) {
        int boy, girl;
        cin >> boy >> girl;
        graph[boy].push_back(girl);
    }

    vector<int> matchLeft(boys + 1, 0);   // matchLeft[nam]  = nữ đang ghép (0 nếu chưa)
    vector<int> matchRight(girls + 1, 0); // matchRight[nữ]  = nam đang ghép (0 nếu chưa)
    vector<int> visited(girls + 1, 0);    // token đánh dấu nữ đã thăm trong lần DFS hiện tại

    // Kuhn: tìm augmenting path bằng DFS bắt đầu từ một bạn nam.
    function<bool(int, int)> augment = [&](int boy, int token) {
        for (const int girl : graph[boy]) {
            if (visited[girl] == token) {
                continue;   // đã thăm nữ này trong lần tìm kiếm này
            }
            visited[girl] = token;
            // Nữ còn trống, hoặc có thể đẩy nam đang ghép với nữ đó sang nữ khác.
            if (matchRight[girl] == 0 || augment(matchRight[girl], token)) {
                matchLeft[boy] = girl;
                matchRight[girl] = boy;
                return true;
            }
        }
        return false;
    };

    // Lần lượt thử tăng matching cho từng bạn nam.
    int matchingSize = 0;
    for (int boy = 1; boy <= boys; ++boy) {
        if (augment(boy, boy)) {
            ++matchingSize;
        }
    }

    // In số cặp cực đại và danh sách các cặp đã ghép.
    cout << matchingSize << '\n';
    for (int boy = 1; boy <= boys; ++boy) {
        if (matchLeft[boy] != 0) {
            cout << boy << ' ' << matchLeft[boy] << '\n';
        }
    }
    return 0;
}
