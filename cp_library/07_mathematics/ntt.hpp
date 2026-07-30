#pragma once
#include <bits/stdc++.h>
using namespace std;
// NTT (Number Theoretic Transform) — tích chập dưới hai modulus NTT và tích chập nguyên chính xác bằng CRT.
// Khi dùng: nhân đa thức/sinh hàm/tương quan; cần hệ số thật không âm nhỏ hơn tích hai modulus.
// ĐPT: mỗi phép tích chập O((n+m) log(n+m)); bộ nhớ O(n+m).
// Dùng: nttMultiply(a,b); nttMultiplySecond(a,b); nttMultiplyExactNonnegative(a,b);
// Bẫy: exact chỉ nhận hệ số không âm, size NTT <=2²¹ và mỗi hệ số kết quả < tích hai modulus.
// CSES: 2111 2112 2113 2115

// API đầu dùng modulus 998244353, API second dùng 1004535809; API exact ghép hai dư bằng CRT.
const long long NTT_MOD = 998244353;
const long long NTT_SECOND_MOD = 1004535809;
static_assert(
    NTT_MOD <= numeric_limits<long long>::max() / NTT_SECOND_MOD,
    "Tích hai NTT modulus phải vừa long long");
const long long NTT_CRT_MOD_PRODUCT = NTT_MOD * NTT_SECOND_MOD;

namespace ntt_detail {

template <long long Mod>
long long power(long long base, long long exponent) {
    long long result = 1;
    base %= Mod;
    if (base < 0) base += Mod;
    while (exponent > 0) {
        if (exponent & 1LL) result = result * base % Mod;
        base = base * base % Mod;
        exponent >>= 1LL;
    }
    return result;
}

template <long long Mod, int PrimitiveRoot, int MaxLog>
void transform(vector<long long>& values, bool inverse) {
    if (values.size() > (size_t(1) << MaxLog))
        throw length_error("ntt: kích thước vượt bậc căn đơn vị của modulus");
    const int size = static_cast<int>(values.size());
    if (size == 0) return;
    if ((size & (size - 1)) != 0)
        throw invalid_argument("ntt: kích thước phải là lũy thừa của 2");

    // Chuẩn hóa giúp các API modulo dùng được cả hệ số âm hay >= modulus.
    for (long long& value : values) {
        value %= Mod;
        if (value < 0) value += Mod;
    }
    for (int index = 1, reversed = 0; index < size; ++index) {
        int bit = size >> 1;
        for (; reversed & bit; bit >>= 1) reversed ^= bit;
        reversed ^= bit;
        if (index < reversed) swap(values[index], values[reversed]);
    }
    for (int length = 2; length <= size; length <<= 1) {
        long long root = power<Mod>(PrimitiveRoot, (Mod - 1) / length);
        if (inverse) root = power<Mod>(root, Mod - 2);
        for (int start = 0; start < size; start += length) {
            long long multiplier = 1;
            for (int offset = 0; offset < length / 2; ++offset) {
                const long long first = values[start + offset];
                const long long second =
                    values[start + offset + length / 2] * multiplier % Mod;
                long long sum = first + second;
                if (sum >= Mod) sum -= Mod;
                long long difference = first - second;
                if (difference < 0) difference += Mod;
                values[start + offset] = sum;
                values[start + offset + length / 2] = difference;
                multiplier = multiplier * root % Mod;
            }
        }
    }
    if (inverse) {
        const long long inverseSize = power<Mod>(size, Mod - 2);
        for (long long& value : values) value = value * inverseSize % Mod;
    }
}

template <long long Mod, int PrimitiveRoot, int MaxLog>
vector<long long> convolution(vector<long long> first, vector<long long> second) {
    if (first.empty() || second.empty()) return {};
    if (first.size() > numeric_limits<size_t>::max() - (second.size() - 1))
        throw length_error("nttMultiply: độ dài kết quả bị tràn");
    const size_t resultSize = first.size() + second.size() - 1;
    const size_t maximumTransformSize = size_t(1) << MaxLog;
    if (resultSize > maximumTransformSize)
        throw length_error("nttMultiply: tích chập quá dài cho modulus");

    size_t transformSize = 1;
    while (transformSize < resultSize) transformSize <<= 1;
    first.resize(transformSize);
    second.resize(transformSize);
    transform<Mod, PrimitiveRoot, MaxLog>(first, false);
    transform<Mod, PrimitiveRoot, MaxLog>(second, false);
    for (size_t index = 0; index < transformSize; ++index)
        first[index] = first[index] * second[index] % Mod;
    transform<Mod, PrimitiveRoot, MaxLog>(first, true);
    first.resize(resultSize);
    return first;
}

// Các phép bão hòa tại cap chỉ dùng để chứng minh một cận trên, không làm toán
// trên hệ số thật. Mọi toán hạng trước phép nhân đều đã được chặn bởi cap.
inline __int128 saturatedAdd(__int128 first, __int128 second, __int128 cap) {
    if (first >= cap || second >= cap - first) return cap;
    return first + second;
}

inline __int128 saturatedMultiply(__int128 first, __int128 second, __int128 cap) {
    if (first == 0 || second == 0) return 0;
    if (first >= cap || second >= cap || first > (cap - 1) / second) return cap;
    return first * second;
}

// Cận đúng cho từng hệ số c[k]:
// c[k] <= min(sum(a)*max(b), sum(b)*max(a), min(n,m)*max(a)*max(b)).
inline __int128 nonnegativeCoefficientBound(const vector<long long>& first,
                                            const vector<long long>& second) {
    const __int128 cap = static_cast<__int128>(NTT_CRT_MOD_PRODUCT);
    __int128 firstSum = 0, secondSum = 0;
    long long firstMaximum = 0, secondMaximum = 0;
    for (long long value : first) {
        if (value < 0)
            throw invalid_argument("nttMultiplyExactNonnegative: có hệ số âm");
        firstMaximum = max(firstMaximum, value);
        firstSum = saturatedAdd(firstSum, value, cap);
    }
    for (long long value : second) {
        if (value < 0)
            throw invalid_argument("nttMultiplyExactNonnegative: có hệ số âm");
        secondMaximum = max(secondMaximum, value);
        secondSum = saturatedAdd(secondSum, value, cap);
    }
    const __int128 byFirstSum =
        saturatedMultiply(firstSum, secondMaximum, cap);
    const __int128 bySecondSum =
        saturatedMultiply(secondSum, firstMaximum, cap);
    const __int128 overlap = static_cast<__int128>(
        min(first.size(), second.size()));
    const __int128 byOverlap = saturatedMultiply(
        saturatedMultiply(overlap, firstMaximum, cap), secondMaximum, cap);
    return min(byFirstSum, min(bySecondSum, byOverlap));
}

}  // namespace ntt_detail

// Ý nghĩa: NTT tại chỗ dưới 998244353 (API cũ).
// Tham số: a = mảng độ dài lũy thừa 2, tối đa 2^23; inv = biến đổi ngược.
// Sau lời gọi: mọi giá trị nằm trong [0, NTT_MOD), kể cả khi input chưa chuẩn hóa.
inline void ntt(vector<long long>& a, bool inv) {
    ntt_detail::transform<NTT_MOD, 3, 23>(a, inv);
}

// Ý nghĩa: nhân hai đa thức dưới modulo 998244353 (API cũ).
// Trả về: c[k] = sum a[i]*b[k-i] mod NTT_MOD; rỗng nếu một input rỗng.
inline vector<long long> nttMultiply(vector<long long> a, vector<long long> b) {
    return ntt_detail::convolution<NTT_MOD, 3, 23>(
        std::move(a), std::move(b));
}

// Ý nghĩa: NTT tại chỗ dưới modulus độc lập thứ hai 1004535809.
// Tham số: a = mảng độ dài lũy thừa 2, tối đa 2^21; inv = biến đổi ngược.
inline void nttSecond(vector<long long>& a, bool inv) {
    ntt_detail::transform<NTT_SECOND_MOD, 3, 21>(a, inv);
}

// Ý nghĩa: nhân hai đa thức dưới modulo thứ hai 1004535809.
// Trả về: c[k] = sum a[i]*b[k-i] mod NTT_SECOND_MOD.
inline vector<long long> nttMultiplySecond(vector<long long> a,
                                           vector<long long> b) {
    return ntt_detail::convolution<NTT_SECOND_MOD, 3, 21>(
        std::move(a), std::move(b));
}

// Ý nghĩa: tích chập số nguyên không âm chính xác bằng hai NTT và CRT.
// Tham số: a, b = hệ số long long không âm.
// Trả về: hệ số nguyên thật; ném overflow_error nếu cận tự tính không nhỏ hơn
// NTT_MOD*NTT_SECOND_MOD, vì khi đó hai phần dư chưa đủ để đảm bảo duy nhất.
inline vector<long long> nttMultiplyExactNonnegative(
        const vector<long long>& a, const vector<long long>& b) {
    if (a.empty() || b.empty()) return {};
    if (a.size() > numeric_limits<size_t>::max() - (b.size() - 1))
        throw length_error(
            "nttMultiplyExactNonnegative: độ dài kết quả bị tràn");
    if (a.size() + b.size() - 1 > (size_t(1) << 21))
        throw length_error(
            "nttMultiplyExactNonnegative: tích chập quá dài cho CRT");
    const __int128 coefficientBound =
        ntt_detail::nonnegativeCoefficientBound(a, b);
    const __int128 crtRange = static_cast<__int128>(NTT_CRT_MOD_PRODUCT);
    if (coefficientBound >= crtRange)
        throw overflow_error(
            "nttMultiplyExactNonnegative: cận hệ số vượt miền CRT");

    const vector<long long> first = nttMultiply(a, b);
    const vector<long long> second = nttMultiplySecond(a, b);
    static const long long inverseFirstModuloSecond =
        ntt_detail::power<NTT_SECOND_MOD>(
            NTT_MOD % NTT_SECOND_MOD, NTT_SECOND_MOD - 2);

    vector<long long> result(first.size());
    for (size_t index = 0; index < result.size(); ++index) {
        long long difference = second[index] - first[index];
        if (difference < 0) difference += NTT_SECOND_MOD;
        const long long multiplier = static_cast<long long>(
            static_cast<__int128>(difference) * inverseFirstModuloSecond %
            NTT_SECOND_MOD);
        const __int128 value =
            first[index] + static_cast<__int128>(NTT_MOD) * multiplier;
        // Kiểm tra hậu điều kiện: đồng thời bảo vệ nếu cận trên bị sửa sai.
        if (value > coefficientBound)
            throw logic_error("nttMultiplyExactNonnegative: CRT vượt cận hệ số");
        result[index] = static_cast<long long>(value);
    }
    return result;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ ntt.hpp -o demo && ./demo
int main() {
    auto moduloProduct = nttMultiply({1, 2}, {3, 4});
    auto exactProduct =
        nttMultiplyExactNonnegative({1000000000LL, 2}, {100000000LL, 4});
    printf("(1+2x)(3+4x) mod 998244353:");
    for (long long value : moduloProduct) printf(" %lld", value); // 3 10 8
    printf("\nTích chính xác:");
    for (long long value : exactProduct)
        printf(" %lld", value); // 100000000000000000 4200000000 8
    printf("\n");
    return 0;
}
#endif
