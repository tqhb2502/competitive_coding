#include <array>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

// Thêm cung kéo theo from -> to vào đồ thị thuận và đồ thị đảo (dùng cho Kosaraju).
void addImplication(int from, int to, vector<vector<int>>& graph,
                    vector<vector<int>>& reverseGraph) {
    graph[from].push_back(to);
    reverseGraph[to].push_back(from);
}

// Cấm việc đồng thời chọn (firstVariable=firstChoice) và (secondVariable=secondChoice):
// mệnh đề 2-SAT (!u hoặc !v) sinh hai cung kéo theo u -> !v và v -> !u.
void forbidPair(int firstVariable, int firstChoice, int secondVariable,
                int secondChoice, vector<vector<int>>& graph,
                vector<vector<int>>& reverseGraph) {
    const int firstLiteral = 2 * firstVariable + firstChoice;
    const int secondLiteral = 2 * secondVariable + secondChoice;
    addImplication(firstLiteral, secondLiteral ^ 1, graph, reverseGraph);
    addImplication(secondLiteral, firstLiteral ^ 1, graph, reverseGraph);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int rows;
    int columns;
    cin >> rows >> columns;
    vector<string> original(rows);
    for (string& row : original) {
        cin >> row;
    }

    // Mỗi ô là một biến Boolean; đỉnh 2*var và 2*var+1 là hai literal của biến đó.
    const int variables = rows * columns;
    const int vertices = 2 * variables;

    // Với mỗi ô, hai màu được phép là hai màu khác màu cũ của ô.
    vector<array<char, 2>> choices(variables);
    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
            const int variable = row * columns + column;
            int count = 0;
            for (char color = 'A'; color <= 'C'; ++color) {
                if (color != original[row][column]) {
                    choices[variable][count++] = color;
                }
            }
        }
    }

    // Dựng đồ thị kéo theo của 2-SAT: với mỗi cặp ô kề cạnh (phải và xuống),
    // cấm mọi cặp lựa chọn khiến hai ô có cùng màu.
    vector<vector<int>> graph(vertices);
    vector<vector<int>> reverseGraph(vertices);
    const array<pair<int, int>, 2> directions = {{{1, 0}, {0, 1}}};
    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
            const int first = row * columns + column;
            for (const auto [rowDelta, columnDelta] : directions) {
                const int nextRow = row + rowDelta;
                const int nextColumn = column + columnDelta;
                if (nextRow >= rows || nextColumn >= columns) {
                    continue;
                }
                const int second = nextRow * columns + nextColumn;
                for (int firstChoice = 0; firstChoice < 2; ++firstChoice) {
                    for (int secondChoice = 0; secondChoice < 2;
                         ++secondChoice) {
                        if (choices[first][firstChoice] ==
                            choices[second][secondChoice]) {
                            forbidPair(first, firstChoice, second, secondChoice,
                                       graph, reverseGraph);
                        }
                    }
                }
            }
        }
    }

    // Kosaraju bước 1: DFS lặp trên đồ thị thuận, ghi thứ tự hoàn thành các đỉnh.
    vector<char> seen(vertices, false);
    vector<int> order;
    order.reserve(vertices);
    for (int start = 0; start < vertices; ++start) {
        if (seen[start]) {
            continue;
        }
        vector<pair<int, int>> stack;
        stack.push_back({start, 0});
        seen[start] = true;
        while (!stack.empty()) {
            const int vertex = stack.back().first;
            int& edgeIndex = stack.back().second;
            if (edgeIndex < static_cast<int>(graph[vertex].size())) {
                const int next = graph[vertex][edgeIndex++];
                if (!seen[next]) {
                    seen[next] = true;
                    stack.push_back({next, 0});
                }
            } else {
                order.push_back(vertex);
                stack.pop_back();
            }
        }
    }

    // Kosaraju bước 2: duyệt các đỉnh theo thứ tự hoàn thành ngược trên đồ thị
    // đảo để gán số thành phần liên thông mạnh (SCC) cho từng đỉnh.
    vector<int> component(vertices, -1);
    int componentCount = 0;
    for (auto iterator = order.rbegin(); iterator != order.rend(); ++iterator) {
        const int start = *iterator;
        if (component[start] != -1) {
            continue;
        }
        vector<int> stack = {start};
        component[start] = componentCount;
        while (!stack.empty()) {
            const int vertex = stack.back();
            stack.pop_back();
            for (const int next : reverseGraph[vertex]) {
                if (component[next] == -1) {
                    component[next] = componentCount;
                    stack.push_back(next);
                }
            }
        }
        ++componentCount;
    }

    // Với mỗi biến, nếu hai literal cùng SCC thì vô nghiệm; ngược lại chọn literal
    // có số SCC lớn hơn, tức SCC đứng sau trong thứ tự tô-pô (quy tắc gán của 2-SAT).
    vector<int> selectedChoice(variables, 0);
    for (int variable = 0; variable < variables; ++variable) {
        if (component[2 * variable] == component[2 * variable + 1]) {
            cout << "IMPOSSIBLE\n";
            return 0;
        }
        selectedChoice[variable] =
            component[2 * variable + 1] > component[2 * variable] ? 1 : 0;
    }

    // In bảng kết quả: mỗi ô lấy màu ứng với lựa chọn đã chọn.
    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
            const int variable = row * columns + column;
            cout << choices[variable][selectedChoice[variable]];
        }
        cout << '\n';
    }
}
