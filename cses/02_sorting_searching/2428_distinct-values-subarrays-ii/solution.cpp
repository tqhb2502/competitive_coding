#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, maximumDistinct;
    cin >> n >> maximumDistinct;

    // Nén tọa độ: ánh xạ mỗi giá trị gốc về một chỉ số trong [0, distinctValues)
    // để về sau dùng mảng đếm tần suất (truy cập theo chỉ số nhanh hơn hash map).
    vector<int> values(n);
    unordered_map<int, int> compressedIndex;
    compressedIndex.reserve(2 * n);
    compressedIndex.max_load_factor(0.7F);
    int distinctValues = 0;
    for (int& value : values) {
        int original;
        cin >> original;
        const auto [iterator, inserted] =
            compressedIndex.emplace(original, distinctValues);
        if (inserted) {
            ++distinctValues;
        }
        value = iterator->second;
    }

    // Two-pointer / sliding window: duy trì cửa sổ [left, right] có <= k giá trị phân biệt.
    vector<int> frequency(distinctValues, 0);
    int windowDistinct = 0;
    int left = 0;
    long long answer = 0;

    for (int right = 0; right < n; ++right) {
        // Thêm phần tử bên phải; nếu trước đó tần suất bằng 0 thì có thêm một giá trị mới.
        if (frequency[values[right]]++ == 0) {
            ++windowDistinct;
        }
        // Thu nhỏ cửa sổ từ bên trái cho tới khi số giá trị phân biệt <= k.
        while (windowDistinct > maximumDistinct) {
            if (--frequency[values[left]] == 0) {
                --windowDistinct;
            }
            ++left;
        }
        // Mọi subarray kết thúc tại right, bắt đầu trong [left, right] đều hợp lệ.
        answer += right - left + 1;
    }

    cout << answer << '\n';
    return 0;
}
