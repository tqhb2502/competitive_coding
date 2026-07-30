#include <functional>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Đọc độ dài thanh ban đầu và số đoạn cần cắt.
    long long totalLength = 0;
    int pieceCount = 0;
    cin >> totalLength >> pieceCount;
    static_cast<void>(totalLength);  // Độ dài ban đầu không cần dùng trong lời giải Huffman.

    // Min-heap chứa độ dài các đoạn, để luôn lấy được hai đoạn ngắn nhất.
    priority_queue<long long, vector<long long>, greater<long long>> pieces;
    for (int index = 0; index < pieceCount; ++index) {
        long long length = 0;
        cin >> length;
        pieces.push(length);
    }

    // Ghép Huffman: mỗi bước gộp hai đoạn ngắn nhất, cộng độ dài tổng vào chi phí.
    long long answer = 0;
    while (pieces.size() > 1U) {
        const long long first = pieces.top();
        pieces.pop();
        const long long second = pieces.top();
        pieces.pop();
        answer += first + second;       // Chi phí của lần cắt tương ứng.
        pieces.push(first + second);    // Đẩy đoạn vừa ghép trở lại hàng đợi.
    }

    cout << answer << '\n';
}
