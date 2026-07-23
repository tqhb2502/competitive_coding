#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <tuple>
#include <utility>
#include <vector>

struct Customer {
    int arrival;
    int departure;
    int original_index;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::vector<Customer> customers(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> customers[i].arrival >> customers[i].departure;
        customers[i].original_index = i;
    }

    // Sắp xếp khách theo ngày đến tăng dần (phá hoà bằng ngày đi rồi chỉ số gốc)
    std::sort(customers.begin(), customers.end(), [](const Customer& first,
                                                     const Customer& second) {
        return std::tie(first.arrival, first.departure, first.original_index) <
               std::tie(second.arrival, second.departure, second.original_index);
    });

    // Min-heap các phòng đang dùng: mỗi phần tử là (ngày đi, số phòng),
    // đỉnh heap là phòng có ngày đi nhỏ nhất (dễ trống nhất)
    std::priority_queue<std::pair<int, int>,
                        std::vector<std::pair<int, int>>,
                        std::greater<std::pair<int, int>>> occupied_rooms;
    std::vector<int> assigned_room(n);
    int room_count = 0;

    for (const Customer& customer : customers) {
        int room;
        // Chỉ tái sử dụng phòng khi khách trước đã đi trước ngày khách này đến
        if (!occupied_rooms.empty() &&
            occupied_rooms.top().first < customer.arrival) {
            room = occupied_rooms.top().second;
            occupied_rooms.pop();
        } else {
            // Không phòng nào kịp trống -> mở phòng mới
            room = ++room_count;
        }

        // Gán phòng theo chỉ số gốc rồi đưa phòng (kèm ngày đi mới) trở lại heap
        assigned_room[customer.original_index] = room;
        occupied_rooms.emplace(customer.departure, room);
    }

    std::cout << room_count << '\n';
    for (int i = 0; i < n; ++i) {
        if (i > 0) {
            std::cout << ' ';
        }
        std::cout << assigned_room[i];
    }
    std::cout << '\n';

    return 0;
}
