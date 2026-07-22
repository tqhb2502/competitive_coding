#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int road_length;
    int light_count;
    std::cin >> road_length >> light_count;

    std::vector<int> insertion_order(light_count);
    for (int& position : insertion_order) {
        std::cin >> position;
    }

    std::vector<int> coordinates = insertion_order;
    coordinates.push_back(0);
    coordinates.push_back(road_length);
    std::sort(coordinates.begin(), coordinates.end());

    const int coordinate_count = static_cast<int>(coordinates.size());
    std::vector<int> left(coordinate_count);
    std::vector<int> right(coordinate_count);
    std::unordered_map<int, int> index_of;
    index_of.reserve(static_cast<std::size_t>(coordinate_count) * 2);

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
    answer[light_count - 1] = current_maximum;

    // Removing insertion_order[k] changes state k + 1 into state k.
    for (int k = light_count - 1; k >= 1; --k) {
        const int node = index_of.at(insertion_order[k]);
        const int left_neighbor = left[node];
        const int right_neighbor = right[node];

        right[left_neighbor] = right_neighbor;
        left[right_neighbor] = left_neighbor;

        current_maximum = std::max(
            current_maximum,
            coordinates[right_neighbor] - coordinates[left_neighbor]);
        answer[k - 1] = current_maximum;
    }

    for (int i = 0; i < light_count; ++i) {
        if (i > 0) {
            std::cout << ' ';
        }
        std::cout << answer[i];
    }
    std::cout << '\n';

    return 0;
}
