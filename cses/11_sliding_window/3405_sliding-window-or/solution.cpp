#include <cstddef>
#include <cstdint>
#include <cstdio>

namespace {

// Bộ sinh dãy theo mô hình LCG: x[i+1] = (multiplier * x[i] + increment) mod modulus.
// Để lấy phần dư thật nhanh khi lặp tới 10^7 lần, ta tính trước reciprocal của modulus
// rồi lấy dư bằng nhân 128-bit và dịch bit (kiểu Barrett reduction) thay cho phép chia.
class Generator {
public:
    Generator(const std::uint64_t initial, const std::uint64_t multiplier,
              const std::uint64_t increment, const std::uint64_t modulus)
        : current_(initial),
          multiplier_(multiplier),
          increment_(increment),
          modulus_(modulus),
          reciprocal_(static_cast<std::uint64_t>(
              (static_cast<__uint128_t>(1) << 64) / modulus
          )) {}

    // Trả về phần tử hiện tại rồi cập nhật trạng thái cho phần tử kế tiếp.
    std::uint32_t next() {
        const std::uint32_t result = static_cast<std::uint32_t>(current_);
        const std::uint64_t product = multiplier_ * current_ + increment_;
        // Ước lượng thương bằng reciprocal rồi hiệu chỉnh tối đa một lần để lấy đúng dư.
        const std::uint64_t quotient = static_cast<std::uint64_t>(
            (static_cast<__uint128_t>(product) * reciprocal_) >> 64
        );

        current_ = product - quotient * modulus_;
        if (current_ >= modulus_) {
            current_ -= modulus_;
        }
        return result;
    }

private:
    std::uint64_t current_;
    const std::uint64_t multiplier_;
    const std::uint64_t increment_;
    const std::uint64_t modulus_;
    const std::uint64_t reciprocal_;
};

// Biến khối thành suffix OR tại chỗ: block[i] = OR các phần tử từ vị trí i tới hết khối.
void build_suffix_or(std::uint32_t* const block, const int length) {
    std::uint32_t suffix = 0;
    for (int index = length - 1; index >= 0; --index) {
        suffix |= block[index];
        block[index] = suffix;
    }
}

}  // namespace

int main() {
    int n;
    int k;
    unsigned long long input_x;
    unsigned long long input_a;
    unsigned long long input_b;
    unsigned long long input_c;

    if (std::scanf("%d %d", &n, &k) != 2) {
        return 0;
    }
    if (std::scanf("%llu %llu %llu %llu", &input_x, &input_a,
                   &input_b, &input_c) != 4) {
        return 0;
    }

    const std::uint32_t x = static_cast<std::uint32_t>(input_x);
    const std::uint64_t a = static_cast<std::uint64_t>(input_a);
    const std::uint64_t b = static_cast<std::uint64_t>(input_b);
    const std::uint64_t c = static_cast<std::uint64_t>(input_c);

    // Trường hợp đặc biệt c = 1: dãy là x, 0, 0, ... nên chỉ đúng một cửa sổ chứa x,
    // do đó XOR của mọi giá trị OR cửa sổ bằng x với mọi k hợp lệ.
    if (c == 1) {
        std::printf("%u\n", static_cast<unsigned int>(x));
        return 0;
    }

    Generator generator(x, a, b, c);
    std::uint32_t answer = 0;

    // Trường hợp đặc biệt k = 1: mỗi cửa sổ chỉ gồm một phần tử nên đáp án là XOR tất cả.
    if (k == 1) {
        for (int index = 0; index < n; ++index) {
            answer ^= generator.next();
        }
        std::printf("%u\n", static_cast<unsigned int>(answer));
        return 0;
    }

    // Trường hợp đặc biệt k = n: chỉ có một cửa sổ nên đáp án là OR của toàn mảng.
    if (k == n) {
        for (int index = 0; index < n; ++index) {
            answer |= generator.next();
        }
        std::printf("%u\n", static_cast<unsigned int>(answer));
        return 0;
    }

    // Trường hợp tổng quát: chia khối kích thước k, ghép suffix OR của khối trước với
    // prefix OR của khối sau để lấy OR của từng cửa sổ trong O(1).
    std::uint32_t* const block =
        new std::uint32_t[static_cast<std::size_t>(k)];

    // Khối căn chỉnh đầu tiên chính là cửa sổ đầu tiên; gộp OR của nó vào đáp án.
    for (int offset = 0; offset < k; ++offset) {
        block[offset] = generator.next();
        answer |= block[offset];
    }
    build_suffix_or(block, k);

    int generated = k;
    while (generated < n) {
        const int remaining = n - generated;
        const int length = remaining < k ? remaining : k;
        std::uint32_t prefix = 0;

        for (int offset = 0; offset < length; ++offset) {
            const std::uint32_t value = generator.next();
            prefix |= value;

            // block[offset + 1] vẫn đang là hậu tố của khối trước.
            // Ở vị trí cuối của một khối đầy đủ, cửa sổ trùng đúng khối hiện tại
            // nên chỉ mình prefix đã là OR của nó.
            if (offset + 1 < k) {
                answer ^= block[offset + 1] | prefix;
            } else {
                answer ^= prefix;
            }
            block[offset] = value;
        }

        generated += length;
        // Tính lại suffix OR để khối hiện tại đóng vai trò khối trước cho vòng kế tiếp.
        if (generated < n) {
            build_suffix_or(block, k);
        }
    }

    delete[] block;
    std::printf("%u\n", static_cast<unsigned int>(answer));
    return 0;
}
