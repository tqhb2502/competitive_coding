#include <iostream>

using namespace std;

using UnsignedInteger = unsigned long long;

// Đếm số lần chữ số digit xuất hiện trong đoạn 1..lastNumber.
UnsignedInteger countDigit(UnsignedInteger lastNumber, int digit) {
    UnsignedInteger result = 0;
    // Xét từng hàng: đơn vị, chục, trăm, ...
    for (UnsignedInteger factor = 1; factor <= lastNumber; factor *= 10) {
        // Tách số thành phần cao hơn, chữ số tại hàng, phần thấp hơn.
        const UnsignedInteger higher = lastNumber / (factor * 10);
        const int current = static_cast<int>((lastNumber / factor) % 10);
        const UnsignedInteger lower = lastNumber % factor;

        if (digit != 0) {
            // Mỗi chu kỳ đầy đủ góp factor lần chữ số digit.
            result += higher * factor;
            // Cộng thêm phần chu kỳ cuối theo chữ số hiện tại.
            if (current > digit) {
                result += factor;
            } else if (current == digit) {
                result += lower + 1;
            }
        } else if (higher != 0) {
            // Với chữ số 0, trừ một chu kỳ để bỏ qua các số 0 đứng đầu.
            result += (higher - 1) * factor;
            if (current == 0) {
                result += lower + 1;
            } else {
                result += factor;
            }
        }

        // Tránh tràn khi nhân factor thêm 10 ở vòng lặp kế.
        if (factor > lastNumber / 10) {
            break;
        }
    }
    return result;
}

// Kiểm tra: viết tới lastNumber thì không chữ số nào bị bấm quá limit lần.
bool canWrite(UnsignedInteger lastNumber, UnsignedInteger limit) {
    for (int digit = 0; digit <= 9; ++digit) {
        if (countDigit(lastNumber, digit) > limit) {
            return false;
        }
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    unsigned long long inputLimit;
    cin >> inputLimit;
    const UnsignedInteger limit = inputLimit;

    // Nhị phân tìm số lớn nhất còn viết được (điều kiện đơn điệu).
    UnsignedInteger low = 0;
    UnsignedInteger high = 1'000'000'000'000'000'000ULL;
    while (low + 1 < high) {
        const UnsignedInteger middle = (low + high) / 2;
        if (canWrite(middle, limit)) {
            low = middle;
        } else {
            high = middle;
        }
    }

    cout << low << '\n';
}
