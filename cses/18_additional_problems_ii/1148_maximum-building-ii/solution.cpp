#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int rows = 0;
    int columns = 0;
    cin >> rows >> columns;

    const int stride = columns + 2;
    const size_t tableSize = static_cast<size_t>(rows + 2) *
                             static_cast<size_t>(stride);
    vector<long long> slopeDifference(tableSize, 0);
    vector<long long> constantDifference(tableSize, 0);

    const auto position = [stride](int height, int width) {
        return static_cast<size_t>(height) * static_cast<size_t>(stride) +
               static_cast<size_t>(width);
    };
    const auto addLinearRange = [&](int height, int leftWidth, int rightWidth,
                                    long long slope, long long constant) {
        if (leftWidth > rightWidth) {
            return;
        }
        slopeDifference[position(height, leftWidth)] += slope;
        slopeDifference[position(height, rightWidth + 1)] -= slope;
        constantDifference[position(height, leftWidth)] += constant;
        constantDifference[position(height, rightWidth + 1)] -= constant;
    };

    vector<int> histogram(columns, 0);
    vector<int> previousSmaller(columns, -1);
    vector<int> nextSmallerOrEqual(columns, columns);
    vector<int> stack;
    stack.reserve(columns);

    for (int row = 0; row < rows; ++row) {
        string line;
        cin >> line;
        for (int column = 0; column < columns; ++column) {
            if (line[column] == '.') {
                ++histogram[column];
            } else {
                histogram[column] = 0;
            }
        }

        stack.clear();
        for (int column = 0; column < columns; ++column) {
            while (!stack.empty() &&
                   histogram[stack.back()] >= histogram[column]) {
                stack.pop_back();
            }
            previousSmaller[column] = stack.empty() ? -1 : stack.back();
            stack.push_back(column);
        }

        stack.clear();
        for (int column = columns - 1; column >= 0; --column) {
            while (!stack.empty() &&
                   histogram[stack.back()] > histogram[column]) {
                stack.pop_back();
            }
            nextSmallerOrEqual[column] =
                stack.empty() ? columns : stack.back();
            stack.push_back(column);
        }

        for (int column = 0; column < columns; ++column) {
            const int height = histogram[column];
            if (height == 0) {
                continue;
            }
            const int leftChoices = column - previousSmaller[column];
            const int rightChoices = nextSmallerOrEqual[column] - column;
            const int smallerSide = min(leftChoices, rightChoices);
            const int largerSide = max(leftChoices, rightChoices);
            const int maximumWidth = leftChoices + rightChoices - 1;

            addLinearRange(height, 1, smallerSide, 1, 0);
            addLinearRange(height, smallerSide + 1, largerSide, 0,
                           smallerSide);
            addLinearRange(height, largerSide + 1, maximumWidth, -1,
                           leftChoices + rightChoices);
        }
    }

    for (int height = 1; height <= rows; ++height) {
        long long slope = 0;
        long long constant = 0;
        for (int width = 1; width <= columns; ++width) {
            slope += slopeDifference[position(height, width)];
            constant += constantDifference[position(height, width)];
            slopeDifference[position(height, width)] =
                slope * width + constant;
        }
    }

    for (int height = rows; height >= 1; --height) {
        for (int width = 1; width <= columns; ++width) {
            slopeDifference[position(height, width)] +=
                slopeDifference[position(height + 1, width)];
        }
    }

    for (int height = 1; height <= rows; ++height) {
        for (int width = 1; width <= columns; ++width) {
            cout << slopeDifference[position(height, width)]
                 << (width == columns ? '\n' : ' ');
        }
    }
}
