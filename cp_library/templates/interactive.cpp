// Khung bài TƯƠNG TÁC — mỗi lần gửi truy vấn PHẢI flush, nếu không sẽ treo (TLE/IL).
// Biên dịch: g++ -std=c++17 -O2 -Wall -Wextra interactive.cpp -o sol
// BẪY: cin.tie(nullptr) tháo liên kết cin-cout nên cout KHÔNG tự flush trước khi đọc
//      -> phải tự '<< flush' (hoặc bỏ dòng cin.tie nếu muốn an toàn tuyệt đối).
#include <bits/stdc++.h>
using namespace std;

// Ý nghĩa: gửi một truy vấn tới interactor rồi đọc phản hồi; nếu interactor đóng luồng thì THOÁT ngay
//          (đọc thất bại nghĩa là hết phiên — đừng nhầm với giá trị hợp lệ như -1).
// Tham số: value = tham số truy vấn theo định dạng đề.
// Trả về: giá trị interactor trả về.
long long ask(long long value) {
    cout << "? " << value << '\n' << flush;
    long long response = 0;
    if (!(cin >> response)) exit(0);
    return response;
}

// Ý nghĩa: in đáp án cuối cùng (có flush) — sau bước này thường không đọc gì nữa.
// Tham số: result = đáp án.
void answer(long long result) {
    cout << "! " << result << '\n' << flush;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // TODO: đọc tham số đầu bài, rồi dùng ask()/answer().
    // Cố ý KHÔNG gọi ask() ở đây để smoke test (verify.sh chạy với stdin rỗng) không gửi truy vấn.
    return 0;
}
