#include <cstdint>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

// Đếm số bit 0 liên tiếp ở cuối (tương đương __builtin_ctzll),
// dùng để lấy vị trí bit thấp nhất đang bật trong một từ 64 bit.
int trailingZeroCount(uint64_t value) {
    int result = 0;
    if ((value & 0xFFFFFFFFULL) == 0) {
        result += 32;
        value >>= 32;
    }
    if ((value & 0xFFFFULL) == 0) {
        result += 16;
        value >>= 16;
    }
    if ((value & 0xFFULL) == 0) {
        result += 8;
        value >>= 8;
    }
    if ((value & 0xFULL) == 0) {
        result += 4;
        value >>= 4;
    }
    if ((value & 0x3ULL) == 0) {
        result += 2;
        value >>= 2;
    }
    if ((value & 0x1ULL) == 0) {
        ++result;
    }
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> values(n);
    int total = 0;
    for (int& value : values) {
        cin >> value;
        total += value;
    }
    // Muốn còn lại 0 thì phải chia được thành hai nhóm tổng bằng nhau,
    // nên tổng phải chẵn; tổng lẻ là vô nghiệm.
    if (total % 2 != 0) {
        cout << -1 << '\n';
        return 0;
    }

    // Subset-sum bằng bitset (mỗi từ 64 bit): tìm tập con có tổng đúng bằng
    // nửa tổng. reachable đánh dấu các tổng đạt được; parentItem/parentSum lưu
    // phần tử vừa thêm và tổng trước đó để truy vết đường đi tạo ra mỗi tổng.
    const int target = total / 2;
    const int wordCount = target / 64 + 1;
    vector<uint64_t> reachable(wordCount, 0);
    reachable[0] = 1;
    vector<int> parentItem(target + 1, -1);
    vector<int> parentSum(target + 1, -1);

    // Lần lượt thêm từng x_i vào bitset bằng cách dịch trái shift bit;
    // dừng sớm ngay khi target đã đạt tới được.
    for (int item = 0; item < n && parentItem[target] == -1; ++item) {
        const int shift = values[item];
        const int wordShift = shift / 64;
        const int bitShift = shift % 64;
        for (int destination = wordCount - 1; destination >= wordShift;
             --destination) {
            const int source = destination - wordShift;
            uint64_t shifted = reachable[source] << bitShift;
            if (bitShift != 0 && source > 0) {
                shifted |= reachable[source - 1] >> (64 - bitShift);
            }
            // Cắt bỏ các bit vượt quá target ở từ cao nhất.
            if (destination == wordCount - 1 && target % 64 != 63) {
                shifted &= (uint64_t{1} << (target % 64 + 1)) - 1;
            }
            // Những tổng lần đầu đạt tới: ghi lại vết để truy nguồn về sau.
            uint64_t newSums = shifted & ~reachable[destination];
            while (newSums != 0) {
                const int bit = trailingZeroCount(newSums);
                const int sum = destination * 64 + bit;
                parentItem[sum] = item;
                parentSum[sum] = sum - shift;
                newSums &= newSums - 1;
            }
            reachable[destination] |= shifted;
        }
    }

    // Không có tập con nào đạt đúng nửa tổng thì vô nghiệm.
    if (((reachable[target / 64] >> (target % 64)) & 1U) == 0U) {
        cout << -1 << '\n';
        return 0;
    }

    // Truy vết ngược từ target về 0 để đánh dấu phần tử thuộc nhóm thứ nhất.
    vector<char> inFirstGroup(n, false);
    int currentSum = target;
    while (currentSum != 0) {
        const int item = parentItem[currentSum];
        inFirstGroup[item] = true;
        currentSum = parentSum[currentSum];
    }

    // Chia dãy thành hai nhóm có tổng bằng nhau.
    vector<int> firstGroup;
    vector<int> secondGroup;
    for (int index = 0; index < n; ++index) {
        if (inFirstGroup[index]) {
            firstGroup.push_back(values[index]);
        } else {
            secondGroup.push_back(values[index]);
        }
    }

    // Lần lượt lấy a ở nhóm A và b ở nhóm B, in thao tác (a, b) rồi đưa |a - b|
    // trở lại nhóm lớn hơn; khi a = b thì sinh ra một số 0 và đếm lại.
    vector<pair<int, int>> operations;
    int zeroCount = 0;
    while (!firstGroup.empty()) {
        const int first = firstGroup.back();
        const int second = secondGroup.back();
        firstGroup.pop_back();
        secondGroup.pop_back();
        operations.push_back({first, second});
        if (first > second) {
            firstGroup.push_back(first - second);
        } else if (second > first) {
            secondGroup.push_back(second - first);
        } else {
            ++zeroCount;
        }
    }
    // Gộp các số 0 dư lại thành một số 0 duy nhất bằng các thao tác (0, 0).
    for (int index = 1; index < zeroCount; ++index) {
        operations.push_back({0, 0});
    }

    for (const auto [first, second] : operations) {
        cout << first << ' ' << second << '\n';
    }
}
