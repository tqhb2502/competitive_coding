#include <algorithm>
#include <array>
#include <iostream>
#include <set>
#include <tuple>
#include <vector>

using namespace std;

class LeftistHeap {
public:
    explicit LeftistHeap(int size)
        : key_(size + 1, 0), left_(size + 1, 0), right_(size + 1, 0),
          rank_(size + 1, 1) {
        rank_[0] = 0;
    }

    void initializeNode(int node, int key) {
        key_[node] = key;
        left_[node] = 0;
        right_[node] = 0;
        rank_[node] = 1;
    }

    int merge(int first, int second) {
        if (first == 0) {
            return second;
        }
        if (second == 0) {
            return first;
        }
        if (pair<int, int>{key_[second], second} <
            pair<int, int>{key_[first], first}) {
            swap(first, second);
        }
        right_[first] = merge(right_[first], second);
        if (rank_[left_[first]] < rank_[right_[first]]) {
            swap(left_[first], right_[first]);
        }
        rank_[first] = rank_[right_[first]] + 1;
        return first;
    }

    int pop(int root) {
        return merge(left_[root], right_[root]);
    }

    int key(int root) const {
        return key_[root];
    }

private:
    vector<int> key_;
    vector<int> left_;
    vector<int> right_;
    vector<int> rank_;
};

class ParityComponents {
public:
    ParityComponents(int size, LeftistHeap& heap)
        : parent_(size + 1), componentSize_(size + 1, 1),
          parityToParent_(size + 1, 0), heaps_(size + 1, {0, 0}),
          inGlobal_(size + 1, {false, false}), heap_(heap) {
        for (int vertex = 0; vertex <= size; ++vertex) {
            parent_[vertex] = vertex;
        }
    }

    pair<int, int> find(int vertex) {
        if (parent_[vertex] == vertex) {
            return {vertex, 0};
        }
        const auto [root, parentParity] = find(parent_[vertex]);
        parityToParent_[vertex] ^= parentParity;
        parent_[vertex] = root;
        return {root, parityToParent_[vertex]};
    }

    bool unite(int first, int second, int requiredParity) {
        auto [firstRoot, firstParity] = find(first);
        auto [secondRoot, secondParity] = find(second);
        if (firstRoot == secondRoot) {
            return (firstParity ^ secondParity) == requiredParity;
        }

        if (componentSize_[firstRoot] < componentSize_[secondRoot]) {
            swap(firstRoot, secondRoot);
        }
        const int rootParity = firstParity ^ secondParity ^ requiredParity;
        parent_[secondRoot] = firstRoot;
        parityToParent_[secondRoot] = rootParity;
        componentSize_[firstRoot] += componentSize_[secondRoot];

        for (int parity = 0; parity < 2; ++parity) {
            const int destinationParity = parity ^ rootParity;
            heaps_[firstRoot][destinationParity] = heap_.merge(
                heaps_[firstRoot][destinationParity],
                heaps_[secondRoot][parity]);
        }
        return true;
    }

    array<int, 2>& heaps(int root) {
        return heaps_[root];
    }

    array<bool, 2>& globalFlags(int root) {
        return inGlobal_[root];
    }

private:
    vector<int> parent_;
    vector<int> componentSize_;
    vector<int> parityToParent_;
    vector<array<int, 2>> heaps_;
    vector<array<bool, 2>> inGlobal_;
    LeftistHeap& heap_;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int size = 0;
    cin >> size;
    vector<int> permutation(size);
    for (int& value : permutation) {
        cin >> value;
    }

    vector<int> enterTime(size + 1, 0);
    vector<int> leaveTime(size + 1, 0);
    vector<char> entered(size + 1, false);
    int inputIndex = 0;
    int nextOutput = 1;
    int time = 0;
    while (nextOutput <= size) {
        ++time;
        if (entered[nextOutput]) {
            leaveTime[nextOutput] = time;
            ++nextOutput;
        } else {
            const int value = permutation[inputIndex];
            ++inputIndex;
            entered[value] = true;
            enterTime[value] = time;
        }
    }

    LeftistHeap heap(size);
    ParityComponents components(size, heap);
    set<tuple<int, int, int>> globalMinimum;

    const auto removeRoot = [&](int root) {
        auto& roots = components.heaps(root);
        auto& flags = components.globalFlags(root);
        for (int parity = 0; parity < 2; ++parity) {
            if (flags[parity]) {
                globalMinimum.erase(
                    {heap.key(roots[parity]), root, parity});
                flags[parity] = false;
            }
        }
    };

    const auto insertRoot = [&](int root, int suppressedParity) {
        auto& roots = components.heaps(root);
        auto& flags = components.globalFlags(root);
        for (int parity = 0; parity < 2; ++parity) {
            if (parity != suppressedParity && roots[parity] != 0) {
                globalMinimum.emplace(heap.key(roots[parity]), root, parity);
                flags[parity] = true;
            }
        }
    };

    bool possible = true;
    for (const int current : permutation) {
        const int leftEndpoint = enterTime[current];
        const int rightEndpoint = leaveTime[current];

        while (!globalMinimum.empty() &&
               get<0>(*globalMinimum.begin()) <= leftEndpoint) {
            const int root = get<1>(*globalMinimum.begin());
            removeRoot(root);
            auto& roots = components.heaps(root);
            for (int parity = 0; parity < 2; ++parity) {
                while (roots[parity] != 0 &&
                       heap.key(roots[parity]) <= leftEndpoint) {
                    roots[parity] = heap.pop(roots[parity]);
                }
            }
            insertRoot(root, -1);
        }

        while (!globalMinimum.empty() &&
               get<0>(*globalMinimum.begin()) < rightEndpoint) {
            const auto [minimumRight, otherRoot, otherParity] =
                *globalMinimum.begin();
            static_cast<void>(minimumRight);
            const auto [currentRoot, currentParity] = components.find(current);

            if (otherRoot == currentRoot) {
                if (otherParity == currentParity) {
                    possible = false;
                }
                break;
            }

            const int representative =
                components.heaps(otherRoot)[otherParity];
            removeRoot(currentRoot);
            removeRoot(otherRoot);
            if (!components.unite(current, representative, 1)) {
                possible = false;
                break;
            }
            const auto [newRoot, newCurrentParity] = components.find(current);
            insertRoot(newRoot, newCurrentParity ^ 1);
        }
        if (!possible) {
            break;
        }

        const auto [root, currentParity] = components.find(current);
        removeRoot(root);
        heap.initializeNode(current, rightEndpoint);
        auto& roots = components.heaps(root);
        roots[currentParity] = heap.merge(roots[currentParity], current);
        insertRoot(root, -1);
    }

    vector<int> color(size + 1, 0);
    if (possible) {
        for (int value = 1; value <= size; ++value) {
            color[value] = components.find(value).second;
        }

        array<vector<int>, 2> stacks;
        nextOutput = 1;
        const auto drain = [&]() {
            bool changed = true;
            while (changed) {
                changed = false;
                for (int stackIndex = 0; stackIndex < 2; ++stackIndex) {
                    if (!stacks[stackIndex].empty() &&
                        stacks[stackIndex].back() == nextOutput) {
                        stacks[stackIndex].pop_back();
                        ++nextOutput;
                        changed = true;
                        break;
                    }
                }
            }
        };

        for (const int value : permutation) {
            drain();
            stacks[color[value]].push_back(value);
            drain();
        }
        drain();
        possible = nextOutput == size + 1;
    }

    if (!possible) {
        cout << "IMPOSSIBLE\n";
        return 0;
    }
    for (int index = 0; index < size; ++index) {
        cout << color[permutation[index]] + 1
             << (index + 1 == size ? '\n' : ' ');
    }
}
