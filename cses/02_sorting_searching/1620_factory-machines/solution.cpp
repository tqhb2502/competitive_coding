#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Đọc số máy n và số sản phẩm cần làm target
    int n;
    long long target;
    cin >> n >> target;

    // Đọc thời gian k_i (giây/1 sản phẩm) của từng máy
    vector<long long> machine_times(n);
    for (long long& time : machine_times) {
        cin >> time;
    }

    // Chặt nhị phân trên đáp án: tìm thời gian T nhỏ nhất sao cho
    // f(T) = tổng(T / k_i) >= target.
    // Cận trên thô: riêng máy nhanh nhất làm xong target sản phẩm trong
    // min(k) * target giây.
    long long low = 0;
    long long high = *min_element(machine_times.begin(), machine_times.end()) * target;

    while (low < high) {
        const long long middle = low + (high - low) / 2;

        // Đếm tổng sản phẩm làm được trong middle giây; dừng sớm khi đã đủ target
        long long products = 0;
        for (const long long time : machine_times) {
            products += middle / time;
            if (products >= target) {
                break;
            }
        }

        // Đủ thì thu về nửa trái (kể cả middle), thiếu thì loại bỏ nửa trái
        if (products >= target) {
            high = middle;
        } else {
            low = middle + 1;
        }
    }

    cout << low << '\n';
    return 0;
}
