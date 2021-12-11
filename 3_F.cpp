#include <iostream>
#include <vector>
#include <cstdint>

#include <set>

using std::cin;
using std::cout;
using std::vector;

namespace graph {
using Edges = vector<vector<size_t>>;
using Weights = vector<vector<int64_t>>;

int64_t Edmons(const Edges& edges, const Weights& weights, size_t from, size_t to) {
    int64_t result = 0;
    vector<vector<int64_t>> adjacency(edges.size(), vector<int64_t>(edges.size()));
    for (size_t from = 0; from < edges.size(); ++from) {
        for (size_t index = 0; index < edges[from].size(); ++index) {
            adjacency[from][edges[from][index]] = weights[from][index];
        }
    }
    vector<size_t> parent(edges.size(), -1);
    size_t last = from;
    vector<std::pair<size_t, size_t>> stack;  // next, last
    vector<char> visited(edges.size());
    while (true) {
        parent.assign(edges.size(), -1);
        visited.assign(edges.size(), false);
        stack.clear();
        stack.emplace_back(from, from);
        for (size_t position = 0; position < stack.size(); ++position) {
            size_t current = stack[position].first;
            if (visited[current]) {
                continue;
            }
            visited[current] = true;
            last = stack[position].second;
            parent[current] = last;
            for (size_t next = 0; next < edges.size(); ++next) {
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
            int64_t min = INT64_MAX;
            size_t iter = last;

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

}  // namespace graph

void Work() {
    size_t n = 0;
    size_t m = 0;
    cin >> n >> m;
    graph::Edges edges(n);
    graph::Weights weights(n);
    while (m--) {
        size_t from = 0;
        size_t to = 0;
        int64_t weight = 0;
        cin >> from >> to >> weight;
        edges[from - 1].push_back(to - 1);
        weights[from - 1].push_back(weight);
    }
    cout << graph::Edmons(edges, weights, 0, n - 1) << '\n';
}

int main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    Work();
}
