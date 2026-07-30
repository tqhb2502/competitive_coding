#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

// Kiểm tra trạng thái đã ở dạng mọi 'A' đứng trước mọi 'B' (bỏ qua ô trống '.').
bool isSortedState(const string &state) {
    bool seenB = false;
    for (char symbol : state) {
        if (symbol == 'B') {
            seenB = true;
        } else if (symbol == 'A' && seenB) {
            return false;
        }
    }
    return true;
}

// Thực hiện một nước đi: chuyển cặp chữ tại vị trí source vào cặp ô trống "..",
// đồng thời để lại cặp ô trống mới ngay tại source.
string movedState(string state, int source) {
    const int empty = static_cast<int>(state.find(".."));
    const char first = state[source];
    const char second = state[source + 1];
    state[empty] = first;
    state[empty + 1] = second;
    state[source] = '.';
    state[source + 1] = '.';
    return state;
}

// BFS trên toàn bộ trạng thái của đoạn nhỏ (tối đa 8 ô) để tìm chuỗi nước đi đưa
// về dạng đã sắp xếp; trả về false nếu đoạn nhỏ vô nghiệm.
bool findSmallPath(const string &start, vector<int> &moves) {
    if (isSortedState(start)) {
        return true;
    }

    // Lưu trạng thái trước và nước đi đã dùng để truy vết ngược lời giải.
    struct Previous {
        string state;
        int move = -1;
    };

    queue<string> pending;
    unordered_map<string, Previous> previous;
    pending.push(start);
    previous.emplace(start, Previous{});

    string target;
    while (!pending.empty() && target.empty()) {
        const string current = pending.front();
        pending.pop();
        const int length = static_cast<int>(current.size());
        // Thử mọi cặp kề nhau không chứa ô trống làm nguồn của nước đi.
        for (int source = 0; source + 1 < length; ++source) {
            if (current[source] == '.' || current[source + 1] == '.') {
                continue;
            }
            string nextState = movedState(current, source);
            if (previous.find(nextState) != previous.end()) {
                continue;
            }
            previous.emplace(nextState, Previous{current, source});
            if (isSortedState(nextState)) {
                target = std::move(nextState);
                break;
            }
            pending.push(std::move(nextState));
        }
    }

    if (target.empty()) {
        return false;
    }
    // Truy vết ngược từ đích về xuất phát rồi đảo lại để có thứ tự nước đi.
    while (target != start) {
        const Previous &step = previous.at(target);
        moves.push_back(step.move);
        target = step.state;
    }
    reverse(moves.begin(), moves.end());
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    string state;
    cin >> n >> state;

    // answerStates lưu trạng thái sau từng nước đi để in ra cuối cùng.
    vector<string> answerStates;
    const auto makeMove = [&state, &answerStates](int source) {
        state = movedState(state, source);
        answerStates.push_back(state);
    };

    // Tìm một cặp kề nhau trong [low, high] khớp yêu cầu chữ đầu/chữ cuối
    // ('?' nghĩa là không ràng buộc); trả về -1 nếu không có.
    const auto findPair = [&state](int low, int high, char requiredFirst,
                                   char requiredSecond) {
        for (int source = low; source <= high; ++source) {
            if (state[source] == '.' || state[source + 1] == '.') {
                continue;
            }
            if (requiredFirst != '?' && state[source] != requiredFirst) {
                continue;
            }
            if (requiredSecond != '?' && state[source + 1] != requiredSecond) {
                continue;
            }
            return source;
        }
        return -1;
    };

    // Đoạn đang xử lý [left, right]; mỗi vòng cố định một 'A' ở đầu và 'B' ở cuối.
    int left = 0;
    int right = 2 * n - 1;
    bool constructionValid = true;

    while (right - left + 1 > 8 && constructionValid) {
        // Đưa cặp ô trống về đầu đoạn (vị trí left), dùng cặp đệm nếu cần.
        int empty = static_cast<int>(state.find(".."));
        if (empty != left) {
            if (state[left] == '.' || state[left + 1] == '.') {
                const int buffer = findPair(left + 2, right - 1, '?', '?');
                if (buffer == -1) {
                    constructionValid = false;
                    break;
                }
                makeMove(buffer);
            }
            makeMove(left);
        }

        // Cố định một cặp bắt đầu bằng 'A' tại đầu đoạn.
        const int pairStartingA = findPair(left + 1, right - 1, 'A', '?');
        if (pairStartingA == -1) {
            constructionValid = false;
            break;
        }
        makeMove(pairStartingA);

        // Làm đối xứng ở cuối: đưa cặp ô trống về right-1, dùng cặp đệm nếu cần.
        empty = static_cast<int>(state.find(".."));
        if (empty != right - 1) {
            if (state[right - 1] == '.' || state[right] == '.') {
                const int buffer = findPair(left + 1, right - 3, '?', '?');
                if (buffer == -1) {
                    constructionValid = false;
                    break;
                }
                makeMove(buffer);
            }
            makeMove(right - 1);
        }

        // Cố định một cặp kết thúc bằng 'B' tại cuối đoạn.
        const int pairEndingB = findPair(left + 1, right - 2, '?', 'B');
        if (pairEndingB == -1) {
            constructionValid = false;
            break;
        }
        makeMove(pairEndingB);

        // Thu nhỏ đoạn còn lại vào phía trong.
        ++left;
        --right;
    }

    // Giải đoạn nhỏ còn lại (tối đa 8 ô) bằng BFS.
    vector<int> finalMoves;
    if (constructionValid) {
        const string smallState = state.substr(
            static_cast<string::size_type>(left),
            static_cast<string::size_type>(right - left + 1));
        constructionValid = findSmallPath(smallState, finalMoves);
    }
    if (!constructionValid) {
        cout << -1 << '\n';
        return 0;
    }

    // Áp các nước đi của đoạn nhỏ (đổi về chỉ số toàn cục) để hoàn tất lời giải.
    for (int localSource : finalMoves) {
        makeMove(left + localSource);
    }

    // In số bước rồi in trạng thái sau mỗi bước.
    cout << answerStates.size() << '\n';
    for (const string &result : answerStates) {
        cout << result << '\n';
    }
}
