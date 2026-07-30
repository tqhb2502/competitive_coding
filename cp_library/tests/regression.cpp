#include <bits/stdc++.h>
using namespace std;

#include "cp_library/01_foundations/binary-search.hpp"
#include "cp_library/01_foundations/dynamic-bitset.hpp"
#include "cp_library/02_data_structures/fenwick.hpp"
#include "cp_library/02_data_structures/dynamic-segment-tree.hpp"
#include "cp_library/02_data_structures/convex-hull-trick.hpp"
#include "cp_library/02_data_structures/median-maintenance.hpp"
#include "cp_library/02_data_structures/persistent-segment-tree.hpp"
#include "cp_library/02_data_structures/segment-tree.hpp"
#include "cp_library/02_data_structures/treap.hpp"
#include "cp_library/02_data_structures/wavelet-matrix.hpp"
#include "cp_library/02_data_structures/weighted-dsu.hpp"
#include "cp_library/03_graphs/bellman-ford.hpp"
#include "cp_library/03_graphs/dijkstra.hpp"
#include "cp_library/03_graphs/dinic.hpp"
#include "cp_library/03_graphs/edge-orientation.hpp"
#include "cp_library/03_graphs/eulerian-path.hpp"
#include "cp_library/03_graphs/hopcroft-karp.hpp"
#include "cp_library/04_trees/tree-augmentation.hpp"
#include "cp_library/07_mathematics/integer-math.hpp"
#include "cp_library/10_advanced_techniques/k-best-enumeration.hpp"
#include "cp_library/11_greedy/greedy-array.hpp"
#include "cp_library/11_greedy/path-matching.hpp"

namespace {

mt19937_64 rng(0xC5E5B1D1ULL);

[[noreturn]] void fail(const string& message) {
    cerr << "regression failure: " << message << '\n';
    exit(1);
}

void require(bool condition, const string& message) {
    if (!condition) fail(message);
}

long long randomLong(long long low, long long high) {
    return low + (long long)(rng() % (uint64_t)(high - low + 1));
}

void testBinarySearch() {
    for (int iteration = 0; iteration < 2000; ++iteration) {
        long long low = randomLong(-1000, 1000);
        long long high = randomLong(low, 1200);
        long long cut = randomLong(low, high);
        require(firstTrue(low, high, [&](long long x) { return x >= cut; }) == cut,
                "firstTrue random");
        require(lastTrue(low, high, [&](long long x) { return x <= cut; }) == cut,
                "lastTrue random");
        if (high < LLONG_MAX)
            require(firstTrue(low, high, [&](long long) { return false; }) == high + 1,
                    "firstTrue sentinel");
        if (low > LLONG_MIN)
            require(lastTrue(low, high, [&](long long) { return false; }) == low - 1,
                    "lastTrue sentinel");
    }
    require(firstTrue(LLONG_MIN, LLONG_MAX,
                      [](long long x) { return x >= -17; }) == -17,
            "firstTrue full signed range");
    require(lastTrue(LLONG_MIN, LLONG_MAX,
                     [](long long x) { return x <= 29; }) == 29,
            "lastTrue full signed range");
    vector<int> a = {-2, 0, 0, 4};
    for (int x = -4; x <= 6; ++x) {
        require(lowerBoundIndex(a, x) == (int)(lower_bound(a.begin(), a.end(), x) - a.begin()),
                "lowerBoundIndex");
        require(upperBoundIndex(a, x) == (int)(upper_bound(a.begin(), a.end(), x) - a.begin()),
                "upperBoundIndex");
    }
}

void testDynamicBitset() {
    for (int n = 1; n <= 150; ++n) {
        DynamicBitset bits(n);
        vector<char> brute(n, false);
        bits.set(0);
        brute[0] = true;
        for (int step = 0; step < 80; ++step) {
            int shift = (int)randomLong(0, n + 20);
            vector<char> next = brute;
            for (int i = 0; i + shift < n; ++i)
                if (brute[i]) next[i + shift] = true;
            bits.shiftOr(shift);
            brute.swap(next);
            long long expected = accumulate(brute.begin(), brute.end(), 0LL);
            require(bits.count() == expected, "DynamicBitset count");
            for (int i = 0; i < n; ++i)
                require(bits.test(i) == (bool)brute[i], "DynamicBitset shiftOr");
        }
    }
}

void testFenwickAndTreap() {
    constexpr int n = 80;
    Fenwick fenwick(n);
    vector<long long> count(n, 0);
    Treap treap;
    multiset<long long> values;
    const vector<long long> special = {LLONG_MIN, -5, 0, 7, LLONG_MAX};
    for (long long value : special) {
        treap.insert(value);
        values.insert(value);
    }
    for (int iteration = 0; iteration < 3000; ++iteration) {
        int index = (int)randomLong(0, n - 1);
        bool add = count[index] == 0 || (rng() & 1);
        fenwick.add(index, add ? 1 : -1);
        count[index] += add ? 1 : -1;

        long long value = randomLong(-30, 30);
        if ((rng() & 1) || values.empty()) {
            treap.insert(value);
            values.insert(value);
        } else {
            auto it = values.find(value);
            treap.erase(value);
            if (it != values.end()) values.erase(it);
        }

        long long total = accumulate(count.begin(), count.end(), 0LL);
        require(fenwick.lowerBound(0) == 0, "Fenwick lowerBound target zero");
        for (long long k = 1; k <= total + 1; ++k) {
            long long prefix = 0;
            int expected = n;
            for (int i = 0; i < n; ++i) {
                prefix += count[i];
                if (prefix >= k) {
                    expected = i;
                    break;
                }
            }
            require(fenwick.kth(k) == expected, "Fenwick kth");
        }

        require(treap.size() == (int)values.size(), "Treap size");
        int rank = 1;
        for (long long current : values)
            require(treap.kth(rank++) == current, "Treap kth");
        for (long long threshold : {-31LL, -5LL, 0LL, 8LL, 31LL, LLONG_MAX}) {
            int expected = (int)distance(values.begin(), values.lower_bound(threshold));
            require(treap.countLess(threshold) == expected, "Treap countLess");
        }
    }
    treap.erase(LLONG_MAX);
    auto it = values.find(LLONG_MAX);
    require(it != values.end(), "Treap LLONG_MAX setup");
    values.erase(it);
    require(treap.size() == (int)values.size(), "Treap erase LLONG_MAX");
}

void testMedianMaintenance() {
    MedianMaintenance median;
    multiset<long long> brute;
    for (int iteration = 0; iteration < 3000; ++iteration) {
        long long value = randomLong(-50, 50);
        if (brute.empty() || (rng() & 1)) {
            median.add(value);
            brute.insert(value);
        } else {
            bool expected = brute.find(value) != brute.end();
            require(median.erase(value) == expected, "MedianMaintenance erase status");
            if (expected) brute.erase(brute.find(value));
        }
        require(median.size() == (int)brute.size(), "MedianMaintenance size");
        if (brute.empty()) {
            require(median.absoluteCost() == 0, "MedianMaintenance empty cost");
            continue;
        }
        auto it = brute.begin();
        advance(it, ((int)brute.size() - 1) / 2);
        long long middle = *it, cost = 0;
        for (long long x : brute) cost += llabs(x - middle);
        require(median.median() == middle, "MedianMaintenance median");
        require(median.absoluteCost() == cost, "MedianMaintenance cost");
    }
}

long long encodeMatrix(array<int, 4> matrix) {
    long long encoded = 0;
    for (int value : matrix) encoded = encoded * 128 + value;
    return encoded;
}

array<int, 4> decodeMatrix(long long encoded) {
    array<int, 4> matrix{};
    for (int i = 3; i >= 0; --i) {
        matrix[i] = (int)(encoded % 128);
        encoded /= 128;
    }
    return matrix;
}

long long multiplyMatrix(long long leftCode, long long rightCode) {
    auto left = decodeMatrix(leftCode);
    auto right = decodeMatrix(rightCode);
    array<int, 4> result{};
    constexpr int mod = 97;
    result[0] = (left[0] * right[0] + left[1] * right[2]) % mod;
    result[1] = (left[0] * right[1] + left[1] * right[3]) % mod;
    result[2] = (left[2] * right[0] + left[3] * right[2]) % mod;
    result[3] = (left[2] * right[1] + left[3] * right[3]) % mod;
    return encodeMatrix(result);
}

void testSegmentTreeOrder() {
    const long long identity = encodeMatrix({1, 0, 0, 1});
    for (int n = 1; n <= 40; ++n) {
        vector<long long> values(n);
        for (long long& value : values)
            value = encodeMatrix({(int)randomLong(0, 96), (int)randomLong(0, 96),
                                  (int)randomLong(0, 96), (int)randomLong(0, 96)});
        SegTree tree(values, multiplyMatrix, identity);
        for (int iteration = 0; iteration < 100; ++iteration) {
            if (rng() & 1) {
                int index = (int)randomLong(0, n - 1);
                values[index] = encodeMatrix({(int)randomLong(0, 96), (int)randomLong(0, 96),
                                              (int)randomLong(0, 96), (int)randomLong(0, 96)});
                tree.set(index, values[index]);
            }
            int left = (int)randomLong(0, n);
            int right = (int)randomLong(left, n);
            long long expected = identity;
            for (int i = left; i < right; ++i)
                expected = multiplyMatrix(expected, values[i]);
            require(tree.query(left, right) == expected, "SegTree non-commutative order");
        }
    }
}

long long bruteMissingSubsetSum(const vector<long long>& values, int left, int right) {
    vector<long long> sums = {0};
    for (int i = left; i < right; ++i) {
        int oldSize = (int)sums.size();
        for (int j = 0; j < oldSize; ++j) sums.push_back(sums[j] + values[i]);
    }
    sort(sums.begin(), sums.end());
    long long wanted = 1;
    for (long long sum : sums) {
        if (sum == wanted) ++wanted;
        else if (sum > wanted) break;
    }
    return wanted;
}

void testPersistentTreesAndWavelet() {
    for (int n = 1; n <= 12; ++n) {
        for (int iteration = 0; iteration < 100; ++iteration) {
            vector<long long> positive(n);
            vector<int> general(n);
            for (int i = 0; i < n; ++i) {
                positive[i] = randomLong(1, 12);
                general[i] = (rng() % 20 == 0) ? INT_MAX : (int)randomLong(0, 100);
            }
            PersistentSeg persistent(positive);
            WaveletMatrix wavelet(general);
            for (int left = 0; left < n; ++left)
                for (int right = left + 1; right <= n; ++right) {
                    vector<long long> sortedPositive(positive.begin() + left,
                                                     positive.begin() + right);
                    sort(sortedPositive.begin(), sortedPositive.end());
                    for (int k = 1; k <= right - left; ++k)
                        require(persistent.kth(left, right, k) == sortedPositive[k - 1],
                                "PersistentSeg kth");
                    for (long long x = 0; x <= 13; ++x) {
                        long long expected = 0;
                        for (int i = left; i < right; ++i)
                            if (positive[i] <= x) expected += positive[i];
                        require(persistent.sumLessEqual(left, right, x) == expected,
                                "PersistentSeg sumLessEqual");
                    }
                    require(persistent.smallestMissingSubsetSum(left, right) ==
                                bruteMissingSubsetSum(positive, left, right),
                            "PersistentSeg missing subset sum");

                    vector<int> sortedGeneral(general.begin() + left, general.begin() + right);
                    sort(sortedGeneral.begin(), sortedGeneral.end());
                    for (int k = 1; k <= right - left; ++k)
                        require(wavelet.kth(left, right, k) == sortedGeneral[k - 1],
                                "WaveletMatrix kth");
                    for (int x : {0, 1, 50, 101, INT_MAX}) {
                        int expected = 0;
                        for (int i = left; i < right; ++i)
                            expected += general[i] < x;
                        require(wavelet.countLess(left, right, x) == expected,
                                "WaveletMatrix countLess");
                    }
                }
        }
    }

    vector<long long> initial = {2, -1, 4, 8};
    PersistentArraySum versions(initial);
    int copy = versions.copyVersion(0);
    versions.pointAssign(0, 1, 7);
    require(versions.rangeSum(0, 0, 4) == 21, "PersistentArraySum updated version");
    require(versions.rangeSum(copy, 0, 4) == 13, "PersistentArraySum copied version");
}

void testWeightedDsu() {
    WeightedDSU dsu(8);
    vector<long long> potential = {7, -2, 4, 10, 10, -8, 1, 13};
    for (int i = 1; i < 8; ++i)
        require(dsu.addConstraint(i - 1, i, potential[i] - potential[i - 1]),
                "WeightedDSU consistent chain");
    for (int u = 0; u < 8; ++u)
        for (int v = 0; v < 8; ++v)
            require(dsu.difference(u, v) == potential[v] - potential[u],
                    "WeightedDSU difference");
    require(!dsu.addConstraint(0, 7, potential[7] - potential[0] + 1),
            "WeightedDSU contradiction");
}

void validateEulerPath(const vector<pair<int, int>>& edges, const vector<int>& path,
                       bool directed) {
    require(path.size() == edges.size() + 1, "Euler path length");
    multiset<pair<int, int>> remaining;
    for (auto [u, v] : edges) {
        if (!directed && u > v) swap(u, v);
        remaining.insert({u, v});
    }
    for (size_t i = 1; i < path.size(); ++i) {
        int u = path[i - 1], v = path[i];
        if (!directed && u > v) swap(u, v);
        auto it = remaining.find({u, v});
        require(it != remaining.end(), "Euler path edge");
        remaining.erase(it);
    }
    require(remaining.empty(), "Euler path uses every edge");
}

void testEulerAndBellmanFord() {
    {
        vector<pair<int, int>> edges = {{0, 1}, {1, 2}, {2, 0}, {0, 1}, {0, 1}};
        EulerPathUndirected graph(3);
        for (auto [u, v] : edges) graph.addEdge(u, v);
        auto path = graph.eulerPath();
        validateEulerPath(edges, path, false);
    }
    {
        vector<pair<int, int>> edges = {{0, 1}, {1, 2}, {2, 0}, {0, 2}};
        EulerPathDirected graph(3);
        for (auto [u, v] : edges) graph.addEdge(u, v);
        auto path = graph.eulerPath();
        validateEulerPath(edges, path, true);
    }
    vector<tuple<int, int, long long>> edges = {
        {0, 1, 2}, {1, 2, -5}, {2, 0, 1}, {2, 3, 4}
    };
    auto cycle = findNegativeCycle(4, edges);
    require(!cycle.empty() && cycle.front() == cycle.back(), "negative cycle reconstruction");
    map<pair<int, int>, long long> minimumEdge;
    for (auto [u, v, w] : edges) {
        auto key = make_pair(u, v);
        if (!minimumEdge.count(key)) minimumEdge[key] = w;
        else minimumEdge[key] = min(minimumEdge[key], w);
    }
    long long cycleWeight = 0;
    for (size_t i = 1; i < cycle.size(); ++i) {
        auto it = minimumEdge.find({cycle[i - 1], cycle[i]});
        require(it != minimumEdge.end(), "negative cycle edge direction");
        cycleWeight += it->second;
    }
    require(cycleWeight < 0, "negative cycle weight");
    auto distance = bellmanFord(4, edges, 0);
    for (long long value : distance) require(value == -BF_INF, "Bellman-Ford -INF propagation");
}

void testDijkstraWideAddition() {
    vector<vector<pair<int, long long>>> graph(3);
    graph[0].push_back({1, DIJ_INF - 1});
    graph[1].push_back({2, LLONG_MAX});
    graph[0].push_back({2, 5});
    auto distance = dijkstra(graph, 0);
    require(distance[1] == DIJ_INF - 1 && distance[2] == 5,
            "Dijkstra wide candidate addition");
}

void testDinicGuards() {
    Dinic graph(2);
    graph.addEdge(0, 1, 7);
    require(graph.maxflow(0, 1) == 7, "Dinic basic flow");

    bool threw = false;
    try {
        (void)graph.maxflow(0, 0);
    } catch (const invalid_argument&) {
        threw = true;
    }
    require(threw, "Dinic rejects equal source and sink");

    Dinic overflow(2);
    overflow.addEdge(0, 1, LLONG_MAX);
    overflow.addEdge(0, 1, 1);
    threw = false;
    try {
        (void)overflow.maxflow(0, 1);
    } catch (const overflow_error&) {
        threw = true;
    }
    require(threw, "Dinic reports unrepresentable total flow");
}

vector<vector<int>> randomTree(int n) {
    vector<vector<int>> tree(n);
    for (int vertex = 1; vertex < n; ++vertex) {
        int parent = (int)randomLong(0, vertex - 1);
        tree[vertex].push_back(parent);
        tree[parent].push_back(vertex);
    }
    return tree;
}

bool connectedWithoutEdge(int n, const vector<pair<int, int>>& edges, int removed) {
    vector<vector<int>> graph(n);
    for (int i = 0; i < (int)edges.size(); ++i) if (i != removed) {
        auto [u, v] = edges[i];
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
    vector<char> seen(n, false);
    queue<int> queue;
    queue.push(0);
    seen[0] = true;
    while (!queue.empty()) {
        int u = queue.front();
        queue.pop();
        for (int v : graph[u]) if (!seen[v]) {
            seen[v] = true;
            queue.push(v);
        }
    }
    return count(seen.begin(), seen.end(), true) == n;
}

void testTreeAugmentation() {
    for (int n = 2; n <= 80; ++n) {
        for (int iteration = 0; iteration < 100; ++iteration) {
            auto tree = randomTree(n);
            vector<pair<int, int>> edges;
            int leaves = 0;
            for (int u = 0; u < n; ++u) {
                leaves += tree[u].size() == 1;
                for (int v : tree[u]) if (u < v) edges.push_back({u, v});
            }
            auto added = augmentTreeNoBridges(tree);
            require((int)added.size() == (leaves + 1) / 2, "Tree augmentation edge count");
            int oldEdges = (int)edges.size();
            edges.insert(edges.end(), added.begin(), added.end());
            for (int removed = 0; removed < oldEdges; ++removed)
                require(connectedWithoutEdge(n, edges, removed),
                        "Tree augmentation left an original bridge");
        }
    }
}

void testEdgeOrientation() {
    for (int n = 1; n <= 25; ++n) {
        for (int iteration = 0; iteration < 500; ++iteration) {
            vector<pair<int, int>> edges;
            int m = (int)randomLong(0, 50);
            if (n == 1) m = 0;
            for (int i = 0; i < m; ++i) {
                int u = (int)randomLong(0, n - 1);
                int v = (int)randomLong(0, n - 2);
                if (v >= u) ++v;
                edges.push_back({u, v});
            }
            vector<vector<pair<int, int>>> graph(n);
            for (int i = 0; i < m; ++i) {
                auto [u, v] = edges[i];
                graph[u].push_back({v, i});
                graph[v].push_back({u, i});
            }
            vector<char> seen(n, false), usedEdge(m, false);
            bool expected = true;
            for (int root = 0; root < n; ++root) if (!seen[root]) {
                int edgeCount = 0;
                vector<int> stack = {root};
                seen[root] = true;
                while (!stack.empty()) {
                    int u = stack.back();
                    stack.pop_back();
                    for (auto [v, id] : graph[u]) {
                        if (!usedEdge[id]) {
                            usedEdge[id] = true;
                            ++edgeCount;
                        }
                        if (!seen[v]) {
                            seen[v] = true;
                            stack.push_back(v);
                        }
                    }
                }
                expected &= edgeCount % 2 == 0;
            }
            auto result = orientEvenOutdegree(n, edges);
            require(result.possible == expected, "Edge orientation feasibility");
            if (!result.possible) continue;
            require(result.directed.size() == edges.size(), "Edge orientation output size");
            vector<int> outdegree(n, 0);
            for (int i = 0; i < m; ++i) {
                auto [u, v] = result.directed[i];
                auto [a, b] = edges[i];
                require((u == a && v == b) || (u == b && v == a),
                        "Edge orientation changed endpoint");
                ++outdegree[u];
            }
            for (int degree : outdegree)
                require(degree % 2 == 0, "Edge orientation odd outdegree");
        }
    }
}

vector<long long> bruteSubsetSums(const vector<long long>& values) {
    vector<long long> answer;
    for (uint64_t mask = 0; mask < (uint64_t(1) << values.size()); ++mask) {
        long long sum = 0;
        for (int i = 0; i < (int)values.size(); ++i)
            if (mask >> i & 1) sum += values[i];
        answer.push_back(sum);
    }
    sort(answer.begin(), answer.end());
    return answer;
}

vector<long long> brutePathMatching(const vector<long long>& cost) {
    int m = (int)cost.size();
    int maximum = (m + 1) / 2;
    const long long infinity = (long long)4e18;
    vector<long long> answer(maximum + 1, infinity);
    answer[0] = 0;
    for (int mask = 0; mask < (1 << m); ++mask) {
        if (mask & (mask << 1)) continue;
        int cardinality = __builtin_popcount((unsigned)mask);
        long long sum = 0;
        for (int i = 0; i < m; ++i) if (mask >> i & 1) sum += cost[i];
        answer[cardinality] = min(answer[cardinality], sum);
    }
    return answer;
}

void testKBestAndPathMatching() {
    for (int n = 0; n <= 12; ++n) {
        for (int iteration = 0; iteration < 300; ++iteration) {
            vector<long long> values(n);
            for (long long& value : values) value = randomLong(-20, 20);
            auto brute = bruteSubsetSums(values);
            long long k = randomLong(0, (long long)brute.size() + 5);
            auto actual = kSmallestSubsetSums(values, k);
            brute.resize((size_t)min<long long>(k, brute.size()));
            require(actual == brute, "K-best subset sums");
        }
    }
    for (int m = 0; m <= 14; ++m) {
        for (int iteration = 0; iteration < 1000; ++iteration) {
            vector<long long> cost(m);
            for (long long& value : cost) value = randomLong(-30, 30);
            require(minCostPathMatching(cost) == brutePathMatching(cost),
                    "minimum-cost path matching");
        }
    }
}

void testIntegerMath() {
    for (uint64_t value = 0; value <= 100000; ++value) {
        uint64_t root = floorSqrtU64(value);
        require((unsigned __int128)root * root <= value, "integer square root lower");
        require((unsigned __int128)(root + 1) * (root + 1) > value,
                "integer square root upper");
    }
    for (uint64_t value : {UINT64_MAX, UINT64_MAX - 1, uint64_t(1) << 63,
                           (uint64_t(1) << 32) * ((uint64_t(1) << 32) - 2)}) {
        uint64_t root = floorSqrtU64(value);
        require((unsigned __int128)root * root <= value, "large integer square root lower");
        require((unsigned __int128)(root + 1) * (root + 1) > value,
                "large integer square root upper");
    }
    string digits;
    for (int value = 1; digits.size() < 20000; ++value) digits += to_string(value);
    for (int k = 1; k <= (int)digits.size(); ++k)
        require(digitInPositiveIntegers(k) == digits[k - 1] - '0', "digit query");
}

void testGreedyArrayOverflow() {
    vector<long long> coins;
    for (int bit = 0; bit <= 62; ++bit) coins.push_back(1LL << bit);
    coins.push_back(1LL << 62);
    bool threw = false;
    try {
        (void)smallestMissingSubsetSum(coins);
    } catch (const overflow_error&) {
        threw = true;
    }
    require(threw, "smallestMissingSubsetSum reports unrepresentable answer");

    threw = false;
    try {
        (void)minIncrementsNondecreasing({LLONG_MAX, LLONG_MIN});
    } catch (const overflow_error&) {
        threw = true;
    }
    require(threw, "minIncrementsNondecreasing avoids signed overflow");
}

void testDynamicSegmentTreeBounds() {
    bool threw = false;
    try {
        DynSeg invalid(0);
    } catch (const invalid_argument&) {
        threw = true;
    }
    require(threw, "DynSeg rejects empty domain");

    DynSeg tree(10);
    tree.add(9, 7);
    require(tree.sum(0, 10) == 7 && tree.sum(9, 10) == 7,
            "DynSeg boundary update");
    threw = false;
    try {
        tree.add(10, 1);
    } catch (const out_of_range&) {
        threw = true;
    }
    require(threw, "DynSeg rejects out-of-domain update");
}

void testConvexHullTrickInterleaving() {
    CHT hull;
    vector<pair<long long, long long>> lines;
    auto add = [&](long long slope, long long intercept) {
        hull.add(slope, intercept);
        lines.push_back({slope, intercept});
    };
    auto check = [&](long long x) {
        long long expected = LLONG_MAX;
        for (auto [slope, intercept] : lines)
            expected = min(expected, slope * x + intercept);
        require(hull.query(x) == expected, "CHT interleaved add/query");
    };

    add(100, 433);
    check(-98);
    add(98, 51);
    check(-96);
    add(97, -135);
    check(-95);
    add(96, -976);  // xóa nhiều tail, gồm cả tail mà con trỏ từng trỏ tới
    add(94, 456);
    add(92, 127);
    check(-92);

    // Các hiệu vẫn đến từ long long nhưng tích chéo có thể lớn hơn signed __int128.
    CHT extreme;
    extreme.add(LLONG_MAX, LLONG_MIN);
    extreme.add(LLONG_MIN + 1, 0);
    extreme.add(LLONG_MIN, LLONG_MAX);
    require(extreme.query(0) == LLONG_MIN, "CHT exact extreme cross product");

    for (int iteration = 0; iteration < 2000; ++iteration) {
        CHT randomHull;
        vector<pair<long long, long long>> randomLines;
        long long slope = 100, x = -100;
        for (int step = 0; step < 100; ++step) {
            if (randomLines.empty() || (rng() & 1)) {
                slope -= randomLong(0, 2);
                long long intercept = randomLong(-1000, 1000);
                randomHull.add(slope, intercept);
                randomLines.push_back({slope, intercept});
            } else {
                x += randomLong(0, 2);
                long long expected = LLONG_MAX;
                for (auto [a, b] : randomLines) expected = min(expected, a * x + b);
                require(randomHull.query(x) == expected, "CHT randomized");
            }
        }
    }
}

int bruteBipartiteMatching(const vector<vector<int>>& adjacency, int rightSize) {
    int leftSize = (int)adjacency.size();
    vector<int> matchRight(rightSize, -1);
    function<int(int)> search = [&](int left) {
        if (left == leftSize) return 0;
        int best = search(left + 1);
        for (int right : adjacency[left]) if (matchRight[right] == -1) {
            matchRight[right] = left;
            best = max(best, 1 + search(left + 1));
            matchRight[right] = -1;
        }
        return best;
    };
    return search(0);
}

void testHopcroftKarp() {
    for (int leftSize = 0; leftSize <= 8; ++leftSize) {
        for (int rightSize = 0; rightSize <= 8; ++rightSize) {
            for (int iteration = 0; iteration < 300; ++iteration) {
                HopcroftKarp matching(leftSize, rightSize);
                vector<vector<int>> adjacency(leftSize);
                for (int left = 0; left < leftSize; ++left) {
                    for (int right = 0; right < rightSize; ++right) {
                        if (rng() % 3 == 0) {
                            matching.addEdge(left, right);
                            adjacency[left].push_back(right);
                        }
                    }
                }
                int expected = bruteBipartiteMatching(adjacency, rightSize);
                require(matching.maxMatching() == expected, "Hopcroft-Karp maximum");
                require(matching.maxMatching() == expected, "Hopcroft-Karp repeated call");
            }
        }
    }
}

}  // namespace

int main() {
    testBinarySearch();
    testDynamicBitset();
    testFenwickAndTreap();
    testMedianMaintenance();
    testSegmentTreeOrder();
    testPersistentTreesAndWavelet();
    testWeightedDsu();
    testEulerAndBellmanFord();
    testDijkstraWideAddition();
    testDinicGuards();
    testTreeAugmentation();
    testEdgeOrientation();
    testKBestAndPathMatching();
    testIntegerMath();
    testGreedyArrayOverflow();
    testDynamicSegmentTreeBounds();
    testConvexHullTrickInterleaving();
    testHopcroftKarp();
    cout << "regression/property tests passed\n";
    return 0;
}
