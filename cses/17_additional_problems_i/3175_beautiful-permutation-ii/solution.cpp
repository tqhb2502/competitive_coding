#include <iostream>

using namespace std;

class Output {
public:
    void add(int value) {
        if (!first_) {
            cout << ' ';
        }
        cout << value;
        first_ = false;
    }

private:
    bool first_ = true;
};

void outputInfinitePrefix(int length, Output& output) {
    int produced = 0;
    for (int start = 1; produced < length; start += 5) {
        const int offsets[5] = {0, 2, 4, 1, 3};
        for (int offset : offsets) {
            if (produced == length) {
                return;
            }
            output.add(start + offset);
            ++produced;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    if (n == 1) {
        cout << "1\n";
        return 0;
    }
    if (n == 2 || n == 3) {
        cout << "NO SOLUTION\n";
        return 0;
    }

    Output output;
    int remainder = n % 5;
    if (remainder == 0 || remainder == 1) {
        outputInfinitePrefix(n, output);
    } else {
        outputInfinitePrefix(n - 4, output);
        if (remainder == 2) {
            output.add(n - 5);
            output.add(n - 1);
            output.add(n - 3);
            output.add(n);
        } else if (remainder == 3) {
            output.add(n - 2);
            output.add(n);
            output.add(n - 4);
            output.add(n - 1);
        } else {
            output.add(n - 2);
            output.add(n);
            output.add(n - 3);
            output.add(n - 1);
        }
    }
    cout << '\n';
}
