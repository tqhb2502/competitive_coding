#include <bits/stdc++.h>
using namespace std;

// Query màu của ghế i; endl flush stdout sau mỗi query (bắt buộc cho interactive).
char ask(int i) {
    cout << "? " << i << endl;   // endl -> flush stdout
    char c;
    cin >> c;                    // đọc 'R' hoặc 'B' (cin bỏ qua khoảng trắng)
    return c;
}

int main() {
    int n;
    cin >> n;

    // Hỏi màu hai đầu của đường thẳng 1..n (không dùng cạnh vòng).
    char c1 = ask(1);
    char cn = ask(n);

    // (n-1) chẵn (n lẻ) => xen kẽ dự đoán color(1) == color(n).
    // Nếu thật sự bằng nhau => ghế 1 và ghế n kề qua cạnh vòng và cùng màu.
    if (c1 == cn) {
        cout << "! " << n << endl;
        return 0;
    }

    // c1 != cn => đoạn [1,n] inconsistent => binary search tìm cặp kề cùng màu.
    // Bất biến: đoạn [lo,hi] luôn inconsistent nên chứa ít nhất một cặp kề cùng màu.
    int lo = 1, hi = n;
    char clo = c1, chi = cn;
    (void)chi; // chi không bắt buộc dùng, giữ lại cho rõ ràng

    while (hi - lo > 1) {
        int mid = (lo + hi) / 2;
        char cm = ask(mid);
        // [lo,mid] nhất quán (consistent) khi: (clo==cm) == ((mid-lo) chẵn).
        bool consistentLeft = ((clo == cm) == (((mid - lo) % 2) == 0));
        if (!consistentLeft) {
            // Nửa trái inconsistent -> thu hẹp về [lo,mid].
            hi = mid;
            chi = cm;
        } else {
            // Nửa trái consistent -> nửa phải [mid,hi] chắc chắn inconsistent.
            lo = mid;
            clo = cm;
        }
    }

    // hi - lo == 1 và [lo,hi] inconsistent => color(lo) == color(hi)
    // => ghế lo và lo+1 cùng màu.
    cout << "! " << lo << endl;
    return 0;
}
