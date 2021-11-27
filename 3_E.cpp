#include <iostream>
#include <vector>
#include <cstdint>

#include <set>

using std::cin;
using std::cout;
using std::vector;

class Graph {
public:
    using Index = size_t;
    using Weight = int64_t;
    using Edges = vector<vector<Index>>;
    using Weights = vector<vector<Weight>>;

public:
    explicit Graph(size_t size) : edges_(size), weights_(size) {
    }

public:
    void Link(Index from, Index to, int64_t weight) {
        edges_[from].push_back(to);
        weights_[from].push_back(weight);
    }
    Weight FordFal(Index from, Index to) const {
        Weight result = 0;
        vector<vector<Weight>> adjacency(edges_.size(), vector<Weight>(edges_.size()));
        for (size_t from = 0; from < edges_.size(); ++from) {
            for (size_t index = 0; index < edges_[from].size(); ++index) {
                adjacency[from][edges_[from][index]] = weights_[from][index];
            }
        }
        vector<Index> parent(edges_.size(), -1);
        auto last = from;
        vector<std::pair<Index, Index>> stack;  // next, last
        auto f_next = &std::pair<Index, Index>::first;
        auto f_last = &std::pair<Index, Index>::second;
        vector<char> visited(edges_.size());
        while (true) {
            parent.assign(edges_.size(), -1);
            visited.assign(edges_.size(), false);
            stack.clear();
            stack.emplace_back(from, from);
            for (size_t position = 0; position < stack.size(); ++position) {
                size_t current = stack[position].*f_next;
                if (visited[current]) {
                    continue;
                }
                visited[current] = true;
                last = stack[position].*f_last;
                parent[current] = last;
                for (size_t next = 0; next < edges_.size(); ++next) {
                    if (!visited[next] && adjacency[current][next] > 0) {
                        stack.emplace_back(next, current);
                    }
                }

                if (current == to) {
                    last = current;
                    break;
                }
            }

            if (last == to) {
                Weight min = INT64_MAX;
                Index iter = last;

                iter = last;
                while (parent[iter] != iter) {
                    if (min > adjacency[parent[iter]][iter]) {
                        min = adjacency[parent[iter]][iter];
                    }
                    iter = parent[iter];
                }

                iter = last;
                while (parent[iter] != iter) {
                    adjacency[iter][parent[iter]] += min;
                    adjacency[parent[iter]][iter] -= min;
                    iter = parent[iter];
                }
                result += min;
            } else {
                return result;
            }
        }
    }

private:
    Edges edges_;
    Weights weights_;
};

void Work() {
    size_t n = 0;
    size_t m = 0;
    cin >> n >> m;
    Graph g(n);
    while (m--) {
        size_t from = 0;
        size_t to = 0;
        int64_t weight = 0;
        cin >> from >> to >> weight;
        g.Link(from - 1, to - 1, weight);
    }
    cout << g.FordFal(0, n - 1) << '\n';
}

int main() {
#ifndef MY
    cin.tie(nullptr)->sync_with_stdio(false);
#endif
    Work();
}
