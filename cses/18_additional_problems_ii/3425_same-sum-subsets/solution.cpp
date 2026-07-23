#include <algorithm>
#include <cstdint>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

using Subset = pair<long long, uint32_t>;

// Sinh mọi cặp (tổng, mask) của các tập con trên một nửa mảng rồi sắp xếp theo
// tổng tăng dần (meet-in-the-middle).
vector<Subset> generateSums(const vector<long long>& values) {
    const int size = static_cast<int>(values.size());
    const uint32_t subsetCount = uint32_t{1} << size;
    vector<Subset> sums(subsetCount);
    sums[0] = {0, 0};
    for (uint32_t mask = 1; mask < subsetCount; ++mask) {
        // Lấy bit thấp nhất của mask để cộng dồn tổng từ mask con nhỏ hơn.
        const uint32_t bit = mask & (~mask + 1U);
        const int index = __builtin_ctz(bit);
        const uint32_t previousMask = mask ^ bit;
        sums[mask] = {sums[previousMask].first + values[index], mask};
    }
    sort(sums.begin(), sums.end());
    return sums;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int size = 0;
    cin >> size;
    vector<long long> values(size);
    for (long long& value : values) {
        cin >> value;
    }

    // Chia mảng thành hai nửa và dựng danh sách tổng của mỗi nửa.
    const int leftSize = size / 2;
    vector<long long> leftValues(values.begin(), values.begin() + leftSize);
    vector<long long> rightValues(values.begin() + leftSize, values.end());
    const vector<Subset> leftSums = generateSums(leftValues);
    const vector<Subset> rightSums = generateSums(rightValues);

    // Đếm số tập con không rỗng có tổng <= limit bằng hai con trỏ trên hai nửa.
    const auto countAtMost = [&](long long limit) {
        long long count = 0;
        int rightIndex = static_cast<int>(rightSums.size()) - 1;
        for (const Subset& leftSubset : leftSums) {
            while (rightIndex >= 0 &&
                   leftSubset.first + rightSums[rightIndex].first > limit) {
                --rightIndex;
            }
            count += static_cast<long long>(rightIndex + 1);
        }
        return count - 1;  // Bỏ tập rỗng.
    };

    // Tìm nhị phân trên vị từ P(v) = [C(v) > v], giữ bất biến P(lower) = false và
    // P(upper) = true. Khi hai đầu liền kề, upper là tổng có ít nhất hai tập con.
    long long lower = 0;
    long long upper = (1LL << size) - 2;
    while (upper - lower > 1) {
        const long long middle = lower + (upper - lower) / 2;
        if (countAtMost(middle) > middle) {
            upper = middle;
        } else {
            lower = middle;
        }
    }
    const long long target = upper;

    // Tìm hai tập con khác nhau có tổng đúng bằng target, mỗi tập tách thành mask
    // nửa trái và mask nửa phải.
    uint32_t firstLeftMask = 0;
    uint32_t firstRightMask = 0;
    uint32_t secondLeftMask = 0;
    uint32_t secondRightMask = 0;
    int foundCount = 0;

    // Hai con trỏ duyệt các cặp (nửa trái, nửa phải) có tổng bằng target.
    int leftIndex = 0;
    int rightIndex = static_cast<int>(rightSums.size()) - 1;
    while (leftIndex < static_cast<int>(leftSums.size()) && rightIndex >= 0) {
        const long long sum = leftSums[leftIndex].first +
                              rightSums[rightIndex].first;
        if (sum < target) {
            ++leftIndex;
            continue;
        }
        if (sum > target) {
            --rightIndex;
            continue;
        }

        // Gom nhóm các tập trái cùng tổng và các tập phải cùng tổng.
        int leftEnd = leftIndex + 1;
        while (leftEnd < static_cast<int>(leftSums.size()) &&
               leftSums[leftEnd].first == leftSums[leftIndex].first) {
            ++leftEnd;
        }
        int rightBegin = rightIndex;
        while (rightBegin > 0 &&
               rightSums[rightBegin - 1].first == rightSums[rightIndex].first) {
            --rightBegin;
        }

        // Số cặp (trái, phải) trong nhóm này cùng cho tổng target.
        const long long combinations =
            static_cast<long long>(leftEnd - leftIndex) *
            static_cast<long long>(rightIndex - rightBegin + 1);
        if (foundCount == 0) {
            // Lấy cặp đầu tiên, và nếu nhóm có >= 2 cặp thì lấy luôn cặp thứ hai
            // bằng cách đổi mask nửa trái hoặc mask nửa phải.
            firstLeftMask = leftSums[leftIndex].second;
            firstRightMask = rightSums[rightBegin].second;
            foundCount = 1;
            if (combinations >= 2 && leftEnd - leftIndex >= 2) {
                secondLeftMask = leftSums[leftIndex + 1].second;
                secondRightMask = rightSums[rightBegin].second;
                foundCount = 2;
            } else if (combinations >= 2) {
                secondLeftMask = leftSums[leftIndex].second;
                secondRightMask = rightSums[rightBegin + 1].second;
                foundCount = 2;
            }
        } else {
            // Đã có cặp đầu ở nhóm tổng khác, lấy thêm cặp thứ hai ở đây.
            secondLeftMask = leftSums[leftIndex].second;
            secondRightMask = rightSums[rightBegin].second;
            foundCount = 2;
        }
        if (foundCount == 2) {
            break;
        }

        // Chuyển sang nhóm tổng kế tiếp.
        leftIndex = leftEnd;
        rightIndex = rightBegin - 1;
    }

    if (foundCount < 2) {
        cout << "IMPOSSIBLE\n";
        return 0;
    }

    // Xóa các bit chung của hai tập để hai phần còn lại rời nhau nhưng vẫn cùng
    // tổng (vì bỏ cùng một lượng ở cả hai bên).
    const uint32_t onlyFirstLeft = firstLeftMask & ~secondLeftMask;
    const uint32_t onlyFirstRight = firstRightMask & ~secondRightMask;
    const uint32_t onlySecondLeft = secondLeftMask & ~firstLeftMask;
    const uint32_t onlySecondRight = secondRightMask & ~firstRightMask;

    // Dựng giá trị hai tập con từ mask nửa trái và nửa phải.
    vector<long long> firstSubset;
    vector<long long> secondSubset;
    for (int index = 0; index < leftSize; ++index) {
        if (((onlyFirstLeft >> index) & 1U) != 0U) {
            firstSubset.push_back(values[index]);
        }
        if (((onlySecondLeft >> index) & 1U) != 0U) {
            secondSubset.push_back(values[index]);
        }
    }
    for (int index = 0; index < size - leftSize; ++index) {
        if (((onlyFirstRight >> index) & 1U) != 0U) {
            firstSubset.push_back(values[leftSize + index]);
        }
        if (((onlySecondRight >> index) & 1U) != 0U) {
            secondSubset.push_back(values[leftSize + index]);
        }
    }

    // In kích thước và các giá trị của từng tập con.
    cout << firstSubset.size() << '\n';
    for (size_t index = 0; index < firstSubset.size(); ++index) {
        cout << firstSubset[index]
             << (index + 1 == firstSubset.size() ? '\n' : ' ');
    }
    cout << secondSubset.size() << '\n';
    for (size_t index = 0; index < secondSubset.size(); ++index) {
        cout << secondSubset[index]
             << (index + 1 == secondSubset.size() ? '\n' : ' ');
    }
}
