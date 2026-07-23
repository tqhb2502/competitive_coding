#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Đọc chiều dài đường và số đèn, rồi đọc mảng đèn theo đúng thứ tự thêm.
    int road_length;
    int light_count;
    std::cin >> road_length >> light_count;

    std::vector<int> insertion_order(light_count);
    for (int& position : insertion_order) {
        std::cin >> position;
    }

    // coords = sắp xếp tăng dần của {0, x} cùng toàn bộ vị trí đèn:
    // đây là trạng thái đầy đủ với hai biên 0, x và tất cả n đèn.
    std::vector<int> coordinates = insertion_order;
    coordinates.push_back(0);
    coordinates.push_back(road_length);
    std::sort(coordinates.begin(), coordinates.end());

    // Dựng doubly-linked list (left/right) trên chỉ số của coords để lấy hàng xóm
    // và gỡ node trong O(1); index_of ánh xạ vị trí đèn -> chỉ số trong coords.
    const int coordinate_count = static_cast<int>(coordinates.size());
    std::vector<int> left(coordinate_count);
    std::vector<int> right(coordinate_count);
    std::unordered_map<int, int> index_of;
    index_of.reserve(static_cast<std::size_t>(coordinate_count) * 2);

    // Đồng thời tính đoạn trống dài nhất ở trạng thái đầy đủ = max hiệu coords liền kề.
    int current_maximum = 0;
    for (int i = 0; i < coordinate_count; ++i) {
        left[i] = i - 1;
        right[i] = i + 1;
        index_of[coordinates[i]] = i;
        if (i > 0) {
            current_maximum = std::max(
                current_maximum, coordinates[i] - coordinates[i - 1]);
        }
    }

    std::vector<int> answer(light_count);
    answer[light_count - 1] = current_maximum;  // đáp án khi có đủ n đèn

    // Xử lý NGƯỢC: gỡ insertion_order[k] để chuyển trạng thái k+1 -> k.
    // Mỗi lần gỡ gộp hai đoạn liền kề nên current_maximum chỉ tăng hoặc giữ nguyên.
    for (int k = light_count - 1; k >= 1; --k) {
        const int node = index_of.at(insertion_order[k]);
        const int left_neighbor = left[node];
        const int right_neighbor = right[node];

        // Xoá node khỏi linked list, hai hàng xóm nối trực tiếp với nhau.
        right[left_neighbor] = right_neighbor;
        left[right_neighbor] = left_neighbor;

        // Đoạn mới gộp lại luôn >= hai đoạn cũ, cập nhật max chạy.
        current_maximum = std::max(
            current_maximum,
            coordinates[right_neighbor] - coordinates[left_neighbor]);
        answer[k - 1] = current_maximum;
    }

    // In đáp án A[1..n] cách nhau bởi dấu cách.
    for (int i = 0; i < light_count; ++i) {
        if (i > 0) {
            std::cout << ' ';
        }
        std::cout << answer[i];
    }
    std::cout << '\n';

    return 0;
}
