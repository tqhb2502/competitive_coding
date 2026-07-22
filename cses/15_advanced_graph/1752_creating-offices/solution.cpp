#include <algorithm>
#include <array>
#include <cstdio>
#include <string>
#include <utility>
#include <vector>

using namespace std;

class FastInput {
public:
    int readInteger() {
        int character = readCharacter();
        while (character != -1 &&
               (character < '0' || character > '9')) {
            character = readCharacter();
        }
        int value = 0;
        while (character >= '0' && character <= '9') {
            value = value * 10 + character - '0';
            character = readCharacter();
        }
        return value;
    }

private:
    static constexpr int BUFFER_SIZE = 1 << 20;
    array<char, BUFFER_SIZE> buffer_{};
    int position_ = 0;
    int length_ = 0;

    int readCharacter() {
        if (position_ == length_) {
            length_ = static_cast<int>(
                fread(buffer_.data(), 1, buffer_.size(), stdin));
            position_ = 0;
            if (length_ == 0) {
                return -1;
            }
        }
        return buffer_[static_cast<size_t>(position_++)];
    }
};

class Tree {
public:
    Tree(int vertexCount, const vector<pair<int, int>>& edges)
        : offsets_(static_cast<size_t>(vertexCount + 1), 0),
          neighbors_(static_cast<size_t>(2 * max(0, vertexCount - 1))) {
        for (const auto [first, second] : edges) {
            ++offsets_[static_cast<size_t>(first + 1)];
            ++offsets_[static_cast<size_t>(second + 1)];
        }
        for (int vertex = 1; vertex <= vertexCount; ++vertex) {
            offsets_[static_cast<size_t>(vertex)] +=
                offsets_[static_cast<size_t>(vertex - 1)];
        }
        vector<int> next = offsets_;
        for (const auto [first, second] : edges) {
            neighbors_[static_cast<size_t>(next[static_cast<size_t>(first)]++)] =
                second;
            neighbors_[static_cast<size_t>(next[static_cast<size_t>(second)]++)] =
                first;
        }
    }

    int begin(int vertex) const {
        return offsets_[static_cast<size_t>(vertex)];
    }

    int end(int vertex) const {
        return offsets_[static_cast<size_t>(vertex + 1)];
    }

    int neighbor(int edgeIndex) const {
        return neighbors_[static_cast<size_t>(edgeIndex)];
    }

private:
    vector<int> offsets_;
    vector<int> neighbors_;
};

class NearestOffice {
public:
    NearestOffice(const Tree& tree, int vertexCount)
        : tree_(tree), vertexCount_(vertexCount),
          centroidAt_(static_cast<size_t>(vertexCount)),
          distanceToCentroid_(static_cast<size_t>(vertexCount)),
          pathLength_(static_cast<size_t>(vertexCount), 0),
          bestDistance_(static_cast<size_t>(vertexCount), INFINITY_DISTANCE) {
        buildCentroidPaths();
    }

    int query(int vertex) const {
        int answer = INFINITY_DISTANCE;
        const int levels = pathLength_[static_cast<size_t>(vertex)];
        for (int level = 0; level < levels; ++level) {
            const int centroid =
                centroidAt_[static_cast<size_t>(vertex)]
                           [static_cast<size_t>(level)];
            answer = min(
                answer,
                bestDistance_[static_cast<size_t>(centroid)] +
                    distanceToCentroid_[static_cast<size_t>(vertex)]
                                       [static_cast<size_t>(level)]);
        }
        return answer;
    }

    void addOffice(int vertex) {
        const int levels = pathLength_[static_cast<size_t>(vertex)];
        for (int level = 0; level < levels; ++level) {
            const int centroid =
                centroidAt_[static_cast<size_t>(vertex)]
                           [static_cast<size_t>(level)];
            int& best = bestDistance_[static_cast<size_t>(centroid)];
            best = min(best,
                       distanceToCentroid_[static_cast<size_t>(vertex)]
                                          [static_cast<size_t>(level)]);
        }
    }

private:
    static constexpr int MAX_CENTROID_LEVELS = 20;
    static constexpr int INFINITY_DISTANCE = 1'000'000'000;

    struct ComponentTask {
        int entry;
        int level;
    };

    struct WalkState {
        int vertex;
        int parent;
        int distance;
    };

    const Tree& tree_;
    int vertexCount_;
    vector<array<int, MAX_CENTROID_LEVELS>> centroidAt_;
    vector<array<int, MAX_CENTROID_LEVELS>> distanceToCentroid_;
    vector<int> pathLength_;
    vector<int> bestDistance_;

    void buildCentroidPaths() {
        vector<char> removed(static_cast<size_t>(vertexCount_), false);
        vector<int> traversalParent(static_cast<size_t>(vertexCount_), -1);
        vector<int> subtreeSize(static_cast<size_t>(vertexCount_), 0);
        vector<int> largestPart(static_cast<size_t>(vertexCount_), 0);

        vector<ComponentTask> tasks;
        tasks.reserve(static_cast<size_t>(vertexCount_));
        tasks.push_back({0, 0});

        vector<int> component;
        component.reserve(static_cast<size_t>(vertexCount_));
        vector<WalkState> walk;
        walk.reserve(static_cast<size_t>(vertexCount_));

        while (!tasks.empty()) {
            const ComponentTask task = tasks.back();
            tasks.pop_back();

            component.clear();
            component.push_back(task.entry);
            traversalParent[static_cast<size_t>(task.entry)] = -1;
            for (size_t index = 0; index < component.size(); ++index) {
                const int vertex = component[index];
                for (int edge = tree_.begin(vertex); edge < tree_.end(vertex);
                     ++edge) {
                    const int next = tree_.neighbor(edge);
                    if (removed[static_cast<size_t>(next)] ||
                        next == traversalParent[static_cast<size_t>(vertex)]) {
                        continue;
                    }
                    traversalParent[static_cast<size_t>(next)] = vertex;
                    component.push_back(next);
                }
            }

            for (const int vertex : component) {
                subtreeSize[static_cast<size_t>(vertex)] = 1;
            }
            for (size_t index = component.size(); index > 1; --index) {
                const int vertex = component[index - 1];
                const int parent =
                    traversalParent[static_cast<size_t>(vertex)];
                subtreeSize[static_cast<size_t>(parent)] +=
                    subtreeSize[static_cast<size_t>(vertex)];
            }

            const int componentSize = static_cast<int>(component.size());
            for (const int vertex : component) {
                largestPart[static_cast<size_t>(vertex)] =
                    componentSize - subtreeSize[static_cast<size_t>(vertex)];
            }
            for (size_t index = 1; index < component.size(); ++index) {
                const int vertex = component[index];
                const int parent =
                    traversalParent[static_cast<size_t>(vertex)];
                largestPart[static_cast<size_t>(parent)] =
                    max(largestPart[static_cast<size_t>(parent)],
                        subtreeSize[static_cast<size_t>(vertex)]);
            }

            int centroid = task.entry;
            for (const int vertex : component) {
                if (largestPart[static_cast<size_t>(vertex)] <
                    largestPart[static_cast<size_t>(centroid)]) {
                    centroid = vertex;
                }
            }

            walk.clear();
            walk.push_back({centroid, -1, 0});
            while (!walk.empty()) {
                const WalkState state = walk.back();
                walk.pop_back();
                centroidAt_[static_cast<size_t>(state.vertex)]
                           [static_cast<size_t>(task.level)] = centroid;
                distanceToCentroid_[static_cast<size_t>(state.vertex)]
                                   [static_cast<size_t>(task.level)] =
                    state.distance;
                pathLength_[static_cast<size_t>(state.vertex)] = task.level + 1;

                for (int edge = tree_.begin(state.vertex);
                     edge < tree_.end(state.vertex); ++edge) {
                    const int next = tree_.neighbor(edge);
                    if (removed[static_cast<size_t>(next)] ||
                        next == state.parent) {
                        continue;
                    }
                    walk.push_back(
                        {next, state.vertex, state.distance + 1});
                }
            }

            removed[static_cast<size_t>(centroid)] = true;
            for (int edge = tree_.begin(centroid); edge < tree_.end(centroid);
                 ++edge) {
                const int next = tree_.neighbor(edge);
                if (!removed[static_cast<size_t>(next)]) {
                    tasks.push_back({next, task.level + 1});
                }
            }
        }
    }
};

int main() {
    FastInput input;
    const int vertexCount = input.readInteger();
    const int minimumDistance = input.readInteger();

    vector<pair<int, int>> edges;
    edges.reserve(static_cast<size_t>(max(0, vertexCount - 1)));
    for (int edge = 0; edge + 1 < vertexCount; ++edge) {
        const int first = input.readInteger() - 1;
        const int second = input.readInteger() - 1;
        edges.push_back({first, second});
    }
    const Tree tree(vertexCount, edges);

    vector<int> order;
    order.reserve(static_cast<size_t>(vertexCount));
    vector<int> parent(static_cast<size_t>(vertexCount), -1);
    order.push_back(0);
    parent[0] = -2;
    for (size_t index = 0; index < order.size(); ++index) {
        const int vertex = order[index];
        for (int edge = tree.begin(vertex); edge < tree.end(vertex); ++edge) {
            const int next = tree.neighbor(edge);
            if (parent[static_cast<size_t>(next)] == -1) {
                parent[static_cast<size_t>(next)] = vertex;
                order.push_back(next);
            }
        }
    }

    NearestOffice nearestOffice(tree, vertexCount);
    vector<int> offices;
    offices.reserve(static_cast<size_t>(vertexCount));
    for (auto iterator = order.rbegin(); iterator != order.rend(); ++iterator) {
        const int vertex = *iterator;
        if (nearestOffice.query(vertex) >= minimumDistance) {
            offices.push_back(vertex);
            nearestOffice.addOffice(vertex);
        }
    }

    string output = to_string(offices.size()) + '\n';
    for (size_t index = 0; index < offices.size(); ++index) {
        if (index != 0) {
            output.push_back(' ');
        }
        output += to_string(offices[index] + 1);
    }
    output.push_back('\n');
    fwrite(output.data(), 1, output.size(), stdout);
}
