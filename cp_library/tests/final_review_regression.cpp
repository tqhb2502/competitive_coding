#include <bits/stdc++.h>
using namespace std;

#include "cp_library/02_data_structures/dsu.hpp"
#include "cp_library/02_data_structures/fenwick.hpp"
#include "cp_library/02_data_structures/interval-set.hpp"
#include "cp_library/02_data_structures/leftist-heap.hpp"
#include "cp_library/02_data_structures/segment-tree.hpp"
#include "cp_library/02_data_structures/window-mode.hpp"
#include "cp_library/04_trees/binary-lifting.hpp"
#include "cp_library/04_trees/centroid-decomposition.hpp"
#include "cp_library/04_trees/long-path-decomposition.hpp"
#include "cp_library/05_dynamic_programming/knapsack.hpp"
#include "cp_library/05_dynamic_programming/weighted-interval-scheduling.hpp"

namespace {

mt19937_64 rng(0xF1A1C0DEULL);

[[noreturn]] void fail(const string& message) {
    cerr << "final-review regression failure: " << message << '\n';
    exit(1);
}

void require(bool condition, const string& message) {
    if (!condition) fail(message);
}

int randomInt(int low, int high) {
    return low + (int)(rng() % (uint64_t)(high - low + 1));
}

struct Text {
    string value;
};

struct Concatenate {
    Text operator()(const Text& left, const Text& right) const {
        return {left.value + right.value};
    }
};

void testGenericSegmentTree() {
    vector<Text> text = {{"a"}, {"b"}, {"c"}, {"d"}};
    GenericSegTree<Text, Concatenate> words(text, Concatenate{}, Text{""});
    require(words.query(1, 4).value == "bcd", "GenericSegTree non-commutative");
    words.set(2, {"X"});
    require(words.query(0, 4).value == "abXd", "GenericSegTree custom Node");

    for (int iteration = 0; iteration < 300; ++iteration) {
        int n = randomInt(1, 80);
        vector<long long> values(n);
        for (auto& value : values) value = randomInt(-20, 20);
        SegTree maximum(values,
                        [](long long a, long long b) { return max(a, b); },
                        LLONG_MIN);
        for (int step = 0; step < 200; ++step) {
            if (randomInt(0, 2) == 0) {
                int index = randomInt(0, n - 1);
                values[index] = randomInt(-20, 20);
                maximum.set(index, values[index]);
            } else {
                int left = randomInt(0, n);
                long long threshold = randomInt(-25, 25);
                int expected = n;
                for (int i = left; i < n; ++i)
                    if (values[i] >= threshold) {
                        expected = i;
                        break;
                    }
                require(maximum.findFirst(
                            left, [&](long long aggregate) {
                                return aggregate >= threshold;
                            }) == expected,
                        "GenericSegTree findFirst");
            }
        }
    }
}

vector<vector<int>> randomTree(int n) {
    vector<vector<int>> tree(n);
    for (int vertex = 1; vertex < n; ++vertex) {
        int parent = randomInt(0, vertex - 1);
        tree[vertex].push_back(parent);
        tree[parent].push_back(vertex);
    }
    return tree;
}

vector<vector<int>> allTreeDistances(const vector<vector<int>>& tree) {
    int n = (int)tree.size();
    vector<vector<int>> distance(n, vector<int>(n, -1));
    for (int source = 0; source < n; ++source) {
        queue<int> queue;
        queue.push(source);
        distance[source][source] = 0;
        while (!queue.empty()) {
            int u = queue.front();
            queue.pop();
            for (int v : tree[u])
                if (distance[source][v] == -1) {
                    distance[source][v] = distance[source][u] + 1;
                    queue.push(v);
                }
        }
    }
    return distance;
}

void testCentroidNearestAndLongPaths() {
    for (int iteration = 0; iteration < 250; ++iteration) {
        int n = randomInt(1, 60);
        auto tree = randomTree(n);
        auto distance = allTreeDistances(tree);
        CentroidNearest nearest(tree);
        vector<char> active(n, false);
        require(nearest.query(0) == -1, "CentroidNearest initially empty");
        for (int step = 0; step < 150; ++step) {
            int vertex = randomInt(0, n - 1);
            if (randomInt(0, 2) == 0) {
                nearest.activate(vertex);
                active[vertex] = true;
            } else {
                int expected = INT_MAX;
                for (int other = 0; other < n; ++other)
                    if (active[other])
                        expected = min(expected, distance[vertex][other]);
                if (expected == INT_MAX) expected = -1;
                require(nearest.query(vertex) == expected,
                        "CentroidNearest randomized");
            }
        }
        for (int length = 0; length <= n + 1; ++length) {
            long long expected = 0;
            if (length == 0) {
                expected = n;
            } else {
                for (int u = 0; u < n; ++u)
                    for (int v = u + 1; v < n; ++v)
                        expected += distance[u][v] == length;
            }
            require(countPathsOfExactLengthLinear(tree, length) == expected,
                    "long-path decomposition randomized");
        }
    }
}

void testBinaryLifting() {
    for (int iteration = 0; iteration < 300; ++iteration) {
        int n = randomInt(1, 60);
        vector<int> next(n);
        for (int& destination : next) destination = randomInt(0, n - 1);
        BinaryJump jump(next, 200);
        for (int query = 0; query < 200; ++query) {
            int start = randomInt(0, n - 1);
            int steps = randomInt(0, 200);
            int expected = start;
            for (int i = 0; i < steps; ++i) expected = next[expected];
            require(jump.jump(start, steps) == expected,
                    "BinaryJump randomized");
        }
    }

    vector<int> chain(80);
    for (int i = 0; i < 80; ++i) chain[i] = min(i + 1, 79);
    BinaryJump jump(chain, 100);
    for (int start = 0; start < 80; ++start)
        for (int bound = start; bound < 80; ++bound) {
            auto result = jump.jumpWhile(
                start, [&](int state) { return state <= bound; }, 100);
            int expectedSteps = bound == 79 ? 100 : min(bound - start, 100);
            require(result.second == expectedSteps &&
                        result.first == min(start + expectedSteps, 79),
                    "BinaryJump jumpWhile");
        }

    for (int iteration = 0; iteration < 150; ++iteration) {
        int n = randomInt(1, 35);
        vector<vector<pair<int, Text>>> tree(n);
        vector<vector<pair<int, char>>> brute(n);
        for (int vertex = 1; vertex < n; ++vertex) {
            int parent = randomInt(0, vertex - 1);
            char label = (char)('a' + randomInt(0, 4));
            tree[vertex].push_back({parent, {string(1, label)}});
            tree[parent].push_back({vertex, {string(1, label)}});
            brute[vertex].push_back({parent, label});
            brute[parent].push_back({vertex, label});
        }
        TreeLift<Text, Concatenate> lift(tree, Concatenate{}, Text{""});
        for (int query = 0; query < 150; ++query) {
            int source = randomInt(0, n - 1), target = randomInt(0, n - 1);
            vector<int> parent(n, -1);
            vector<char> edgeToParent(n);
            queue<int> queue;
            queue.push(source);
            parent[source] = source;
            while (!queue.empty()) {
                int u = queue.front();
                queue.pop();
                for (auto [v, label] : brute[u])
                    if (parent[v] == -1) {
                        parent[v] = u;
                        edgeToParent[v] = label;
                        queue.push(v);
                    }
            }
            string expected;
            for (int v = target; v != source; v = parent[v])
                expected.push_back(edgeToParent[v]);
            reverse(expected.begin(), expected.end());
            require(lift.pathAggregate(source, target).value == expected,
                    "TreeLift non-commutative path order");
        }
    }
}

void testWeightedScheduling() {
    for (int iteration = 0; iteration < 1000; ++iteration) {
        int n = randomInt(0, 12);
        vector<WeightedInterval> jobs;
        for (int i = 0; i < n; ++i) {
            int left = randomInt(0, 12);
            int right = randomInt(left, 15);
            jobs.push_back({left, right, randomInt(-5, 25), i});
        }
        long long expected = 0;
        for (int mask = 0; mask < (1 << n); ++mask) {
            bool valid = true;
            long long value = 0;
            for (int i = 0; i < n; ++i)
                if (mask >> i & 1) {
                    value += jobs[i].weight;
                    for (int j = 0; j < i; ++j)
                        if ((mask >> j & 1) &&
                            !(jobs[i].finish < jobs[j].start ||
                              jobs[j].finish < jobs[i].start))
                            valid = false;
                }
            if (valid) expected = max(expected, value);
        }
        auto answer = weightedIntervalSchedule(jobs);
        require(answer.value == expected, "weighted interval optimum");
        long long chosenValue = 0;
        for (int id : answer.chosenIds) chosenValue += jobs[id].weight;
        require(chosenValue == answer.value, "weighted interval reconstruction value");
        for (int i = 1; i < (int)answer.chosenIds.size(); ++i)
            require(jobs[answer.chosenIds[i - 1]].finish
                        < jobs[answer.chosenIds[i]].start,
                    "weighted interval reconstruction compatibility");
    }
}

long long bruteBounded(const vector<int>& weight, const vector<long long>& value,
                       const vector<int>& limit, int capacity, int item = 0) {
    if (item == (int)weight.size()) return 0;
    long long answer = 0;
    for (int copies = 0; copies <= limit[item] &&
                         copies * weight[item] <= capacity; ++copies)
        answer = max(answer, copies * value[item] +
            bruteBounded(weight, value, limit,
                         capacity - copies * weight[item], item + 1));
    return answer;
}

void testBoundedKnapsack() {
    for (int iteration = 0; iteration < 1000; ++iteration) {
        int n = randomInt(0, 6), capacity = randomInt(0, 30);
        vector<int> weight(n), limit(n);
        vector<long long> value(n);
        for (int i = 0; i < n; ++i) {
            weight[i] = randomInt(1, 10);
            value[i] = randomInt(-5, 20);
            limit[i] = randomInt(0, 5);
        }
        require(boundedKnapsack(weight, value, limit, capacity) ==
                    bruteBounded(weight, value, limit, capacity),
                "bounded knapsack randomized");
    }
}

void testWindowMode() {
    WindowMode<int> mode;
    map<int, int> count;
    int elements = 0;
    for (int step = 0; step < 10000; ++step) {
        int value = randomInt(-20, 20);
        if (elements == 0 || randomInt(0, 1) == 0) {
            mode.add(value);
            ++count[value];
            ++elements;
        } else {
            bool expected = count[value] > 0;
            require(mode.remove(value) == expected, "WindowMode remove status");
            if (expected) {
                if (--count[value] == 0) count.erase(value);
                --elements;
            }
        }
        require(mode.size() == elements, "WindowMode size");
        if (elements == 0) {
            require(!mode.mode() && mode.maxFrequency() == 0,
                    "WindowMode empty");
        } else {
            int bestFrequency = 0, bestValue = 0;
            for (auto [candidate, frequency] : count)
                if (frequency > bestFrequency) {
                    bestFrequency = frequency;
                    bestValue = candidate;
                }
            require(mode.mode() && *mode.mode() == bestValue &&
                        mode.maxFrequency() == bestFrequency,
                    "WindowMode randomized");
        }
    }
}

void testIntervalSet() {
    IntervalSet<int> intervals;
    constexpr int low = -40, high = 40;
    vector<char> covered(high - low + 1, false);
    for (int step = 0; step < 10000; ++step) {
        int left = randomInt(low, high), right = randomInt(left, high);
        if (randomInt(0, 1) == 0) {
            intervals.insert(left, right);
            for (int x = left; x <= right; ++x) covered[x - low] = true;
        } else {
            intervals.erase(left, right);
            for (int x = left; x <= right; ++x) covered[x - low] = false;
        }
        for (int sample = 0; sample < 5; ++sample) {
            int point = randomInt(low, high);
            require(intervals.contains(point) == (bool)covered[point - low],
                    "IntervalSet contains");
            int queryLeft = randomInt(low, high);
            int queryRight = randomInt(queryLeft, high);
            optional<int> first, last;
            for (int x = queryLeft; x <= queryRight; ++x)
                if (covered[x - low]) {
                    if (!first) first = x;
                    last = x;
                }
            require(intervals.firstCovered(queryLeft, queryRight) == first &&
                        intervals.lastCovered(queryLeft, queryRight) == last,
                    "IntervalSet first/last");
        }
        int previousEnd = INT_MIN;
        for (auto [leftEndpoint, rightEndpoint] : intervals.intervals()) {
            require(leftEndpoint <= rightEndpoint &&
                        (previousEnd == INT_MIN || previousEnd + 1 < leftEndpoint),
                    "IntervalSet canonical disjoint form");
            previousEnd = rightEndpoint;
        }
    }
}

void testLeftistHeapAndBounds() {
    LeftistHeap<int> heap(1000);
    int left = -1, right = -1;
    vector<int> values;
    for (int step = 0; step < 1000; ++step) {
        int value = randomInt(-10000, 10000);
        values.push_back(value);
        if (step & 1) left = heap.push(left, value);
        else right = heap.push(right, value);
    }
    int root = heap.meld(left, right);
    require(heap.size(root) == (int)values.size(), "LeftistHeap size");
    sort(values.begin(), values.end());
    for (int value : values) {
        require(!heap.empty(root) && heap.top(root) == value,
                "LeftistHeap sorted pop");
        root = heap.pop(root);
    }
    require(heap.empty(root), "LeftistHeap final empty");

    bool threw = false;
    Fenwick fenwick(3);
    try { fenwick.add(-1, 1); } catch (const out_of_range&) { threw = true; }
    require(threw, "Fenwick rejects negative index");
    threw = false;
    try { (void)fenwick.sum(4); } catch (const out_of_range&) { threw = true; }
    require(threw, "Fenwick rejects bad prefix");
    threw = false;
    DSU dsu(2);
    try { (void)dsu.find(2); } catch (const out_of_range&) { threw = true; }
    require(threw, "DSU rejects bad vertex");
}

}  // namespace

int main() {
    testGenericSegmentTree();
    testCentroidNearestAndLongPaths();
    testBinaryLifting();
    testWeightedScheduling();
    testBoundedKnapsack();
    testWindowMode();
    testIntervalSet();
    testLeftistHeapAndBounds();
    cout << "final-review regression tests passed\n";
    return 0;
}
