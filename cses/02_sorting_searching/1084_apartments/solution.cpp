#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

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

    std::sort(applicants.begin(), applicants.end());
    std::sort(apartments.begin(), apartments.end());

    int applicant = 0;
    int apartment = 0;
    int matches = 0;

    while (applicant < applicant_count && apartment < apartment_count) {
        if (apartments[apartment] < applicants[applicant] - tolerance) {
            ++apartment;
        } else if (apartments[apartment] > applicants[applicant] + tolerance) {
            ++applicant;
        } else {
            ++matches;
            ++applicant;
            ++apartment;
        }
    }

    std::cout << matches << '\n';
    return 0;
}
