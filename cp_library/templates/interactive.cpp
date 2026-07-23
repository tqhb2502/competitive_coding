#include <iostream>

using namespace std;

long long ask(long long value) {
    cout << "? " << value << '\n' << flush;

    long long response = 0;
    if (!(cin >> response)) {
        return -1;
    }
    return response;
}

void answer(long long result) {
    cout << "! " << result << '\n' << flush;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Cố ý không gọi ask/answer để smoke test không gửi truy vấn ngoài ý muốn.
    return 0;
}
