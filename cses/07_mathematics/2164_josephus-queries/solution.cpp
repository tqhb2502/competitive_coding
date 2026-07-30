#include <iostream>

// Trả về số hiệu (1-indexed) của đứa trẻ bị loại ở lượt thứ k trên vòng tròn n đứa.
// Khử đệ quy theo tầng: mỗi tầng thu vòng tròn về nửa kích thước, đồng thời giữ phép
// biến đổi tuyến tính answer = offset + scale * sublabel để ánh xạ ngược về nhãn gốc.
long long kth_removed(long long n, long long k) {
    long long offset = 0;
    long long scale = 1;

    while (true) {
        // m = số đứa bị loại trong vòng quét đầu (các vị trí chẵn).
        const long long first_round = n / 2;
        // Nếu k rơi vào vòng quét đầu, đáp án là vị trí chẵn thứ k => 2*k.
        if (k <= first_round) {
            return offset + scale * (2 * k);
        }

        // r = chỉ số loại trong vòng nhỏ (chỉ còn các vị trí lẻ sống sót).
        const long long remaining_k = k - first_round;
        if ((n & 1LL) == 0) {
            // n chẵn: các đứa còn lại 1,3,... ánh xạ sublabel x thành 2*x-1.
            offset -= scale;
            scale *= 2;
            n = first_round;
            k = remaining_k;
        } else {
            // n lẻ: sau khi loại hết vị trí chẵn thì số 1 bị loại ngay tiếp theo.
            if (remaining_k == 1) {
                return offset + scale;
            }
            // Phần còn lại 3,5,... ánh xạ sublabel x thành 2*x+1.
            offset += scale;
            scale *= 2;
            n = first_round;
            k = remaining_k - 1;
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int queries;
    std::cin >> queries;
    while (queries-- > 0) {
        long long n, k;
        std::cin >> n >> k;
        std::cout << kth_removed(n, k) << '\n';
    }
    return 0;
}
