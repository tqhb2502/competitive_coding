#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Đọc số ứng viên, số căn hộ và sai số cho phép k (tolerance)
    int applicant_count;
    int apartment_count;
    long long tolerance;
    std::cin >> applicant_count >> apartment_count >> tolerance;

    std::vector<long long> applicants(applicant_count);
    std::vector<long long> apartments(apartment_count);
    for (long long& desired_size : applicants) {
        std::cin >> desired_size;
    }
    for (long long& size : apartments) {
        std::cin >> size;
    }

    // Sắp xếp cả hai mảng tăng dần để chạy greedy bằng two-pointer
    std::sort(applicants.begin(), applicants.end());
    std::sort(apartments.begin(), apartments.end());

    int applicant = 0;
    int apartment = 0;
    int matches = 0;

    // Two-pointer: ghép ứng viên nhỏ nhất chưa ghép với căn hộ vừa vặn nhỏ nhất
    while (applicant < applicant_count && apartment < apartment_count) {
        if (apartments[apartment] < applicants[applicant] - tolerance) {
            // Căn hộ quá nhỏ cho mọi ứng viên còn lại -> bỏ căn hộ này
            ++apartment;
        } else if (apartments[apartment] > applicants[applicant] + tolerance) {
            // Căn hộ quá lớn -> ứng viên này không thể nhận căn hộ nào nữa
            ++applicant;
        } else {
            // Nằm trong đoạn chấp nhận -> ghép một cặp, chuyển cả hai con trỏ
            ++matches;
            ++applicant;
            ++apartment;
        }
    }

    std::cout << matches << '\n';
    return 0;
}
