#include <algorithm>
#include <array>
#include <iostream>

using namespace std;

// Kết quả khi nén một loạt bước greedy trên một khối thập phân.
struct Transition {
    long long steps = 0;   // số bước greedy đã thực hiện
    int deficit = 0;       // độ thiếu hụt: suffix mới trước khi mượn là -deficit
};

array<long long, 19> powerOfTen{};
// Bảng tiền xử lý: completeBlock[k][m][d] là kết quả process cho khối 10^k - d.
Transition completeBlock[19][10][10];

// Chạy các bước greedy (luôn trừ chữ số lớn nhất) trên "digits" chữ số cuối có
// giá trị "suffix", biết chữ số lớn nhất của tiền tố là "prefixMaximum". Dừng
// khi suffix âm (phải mượn 1 từ prefix) và trả về số bước cùng độ thiếu hụt.
Transition processSuffix(int digits, long long suffix, int prefixMaximum) {
    // Trường hợp cơ sở: chỉ còn một chữ số ở suffix.
    if (digits == 1) {
        if (suffix >= prefixMaximum) {
            return {2, prefixMaximum};
        }
        return {1, prefixMaximum - static_cast<int>(suffix)};
    }

    const long long blockSize = powerOfTen[digits - 1];
    int leadingDigit = static_cast<int>(suffix / blockSize);  // chữ số đầu của suffix
    long long remainder = suffix % blockSize;                 // phần thấp còn lại
    long long totalSteps = 0;

    while (true) {
        // Chữ số lớn nhất của ngữ cảnh khi xử lý phần thấp.
        const int contextMaximum = max(prefixMaximum, leadingDigit);
        Transition transition;
        if (remainder >= blockSize - 9) {
            // Nhánh này ứng với một khối 10^(digits-1) - initialDeficit đã tra bảng.
            const int initialDeficit =
                static_cast<int>(blockSize - remainder);
            transition = completeBlock[digits - 1][contextMaximum]
                                      [initialDeficit];
        } else {
            // Nhánh chưa tiền xử lý: đệ quy xử lý trọn phần thấp.
            transition =
                processSuffix(digits - 1, remainder, contextMaximum);
        }
        totalSteps += transition.steps;
        // Đã xử lý hết các chữ số đầu, trả về độ thiếu hụt cuối cùng.
        if (leadingDigit == 0) {
            return {totalSteps, transition.deficit};
        }
        // Chữ số đầu giảm một và phần thấp thành 10^(digits-1) - deficit (phép trừ thập phân).
        --leadingDigit;
        remainder = blockSize - transition.deficit;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Tính trước các lũy thừa của 10.
    powerOfTen[0] = 1;
    for (int exponent = 1; exponent <= 18; ++exponent) {
        powerOfTen[exponent] = powerOfTen[exponent - 1] * 10;
    }
    // Tiền xử lý bảng cho mọi khối 10^digits - deficit với mọi chữ số lớn nhất tiền tố.
    for (int digits = 1; digits <= 18; ++digits) {
        for (int prefixMaximum = 1; prefixMaximum <= 9; ++prefixMaximum) {
            for (int deficit = 1; deficit <= 9; ++deficit) {
                completeBlock[digits][prefixMaximum][deficit] =
                    processSuffix(digits, powerOfTen[digits] - deficit,
                                  prefixMaximum);
            }
        }
    }

    long long number;
    cin >> number;
    long long answer = 0;
    while (number >= 10) {
        // Tách chữ số đầu làm prefix, xử lý toàn bộ suffix bằng process.
        int exponent = 1;
        while (exponent < 18 && powerOfTen[exponent + 1] <= number) {
            ++exponent;
        }
        const long long blockSize = powerOfTen[exponent];
        const int leadingDigit = static_cast<int>(number / blockSize);
        const long long suffix = number % blockSize;
        const Transition transition =
            processSuffix(exponent, suffix, leadingDigit);
        answer += transition.steps;
        // Chữ số đầu giảm một, số mới là leadingDigit*blockSize - deficit.
        number = leadingDigit * blockSize - transition.deficit;
    }
    // Bước cuối cho chữ số còn lại nếu khác 0.
    if (number != 0) {
        ++answer;
    }
    cout << answer << '\n';
}
