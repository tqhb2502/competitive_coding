#include <iostream>
#include <vector>

using namespace std;

constexpr long long MODULO = 1'000'000'007LL;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Đọc n số và đếm tần suất xuất hiện của từng giá trị
    int size = 0;
    cin >> size;
    vector<int> frequency(size + 1, 0);
    for (int index = 0; index < size; ++index) {
        int value = 0;
        cin >> value;
        ++frequency[value];
    }

    // Tính trước các lũy thừa của 2 theo modulo
    vector<long long> powersOfTwo(size + 1, 1);
    for (int count = 1; count <= size; ++count) {
        powersOfTwo[count] = powersOfTwo[count - 1] * 2LL % MODULO;
    }

    // exact[d] = số tập con không rỗng có gcd đúng bằng d
    // Duyệt d giảm dần để mọi bội của d đã được tính trước
    vector<long long> exact(size + 1, 0);
    for (int divisor = size; divisor >= 1; --divisor) {
        // Đếm số phần tử chia hết cho divisor bằng cách duyệt các bội
        int divisibleCount = 0;
        for (int multiple = divisor; multiple <= size; multiple += divisor) {
            divisibleCount += frequency[multiple];
        }

        // Số tập con chỉ gồm các bội của divisor là 2^divisibleCount - 1;
        // trừ đi các tập con có gcd là bội thực sự của divisor
        long long ways = powersOfTwo[divisibleCount] - 1;
        for (int multiple = divisor + divisor; multiple <= size;
             multiple += divisor) {
            ways -= exact[multiple];
            if (ways < 0) {
                ways += MODULO;
            }
        }
        exact[divisor] = ways;
    }

    // In số tập con có gcd bằng 1, 2, ..., n
    for (int divisor = 1; divisor <= size; ++divisor) {
        cout << exact[divisor] << (divisor == size ? '\n' : ' ');
    }
}
