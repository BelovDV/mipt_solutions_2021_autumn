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

int64_t DfsDinic(int64_t flow, vector<vector<int64_t>>& adj, size_t from, size_t to, vector<int64_t>& layout) {
    if (flow == 0) {
        return 0;
    }
    if (from == to) {
        return flow;
    }
    for (size_t next = 0; next < adj.size(); ++next) {
        if (adj[from][next] <= 0 || layout[from] + 1 != layout[next]) {
            continue;
        }
        int64_t try_flow = std::min(flow, adj[from][next]);
        int64_t pushed = DfsDinic(try_flow, adj, next, to, layout);
        if (pushed) {
            adj[from][next] -= pushed;
            adj[next][from] += pushed;
            return pushed;
        }
    }
    return 0;
}

int64_t Dinic(const Edges& edges, const Weights& weights, size_t from, size_t to) {
    vector<vector<int64_t>> adjacency(edges.size(), vector<int64_t>(edges.size()));
    for (size_t from = 0; from < edges.size(); ++from) {
        for (size_t index = 0; index < edges[from].size(); ++index) {
            adjacency[from][edges[from][index]] = weights[from][index];
        }
    }
    int64_t result = 0;
    vector<std::pair<size_t, size_t>> stack;  // next, lay
    while (true) {
        stack.clear();

        const int64_t zero = -2;
        vector<int64_t> layout(edges.size(), zero);
        stack.emplace_back(from, 0);
        size_t iter = 0;
        while (iter < stack.size()) {
            size_t current = stack[iter].first;
            if (layout[current] != zero) {
                ++iter;
                continue;
            }
            layout[current] = stack[iter].second;
            for (size_t next = 0; next < edges.size(); ++next) {
                if (adjacency[current][next] > 0) {
                    if (layout[next] == zero) {
                        stack.emplace_back(next, layout[current] + 1);
                    }
                }
            }
            ++iter;
        }
        if (layout[to] == zero) {
            return result;
        }

        while (int64_t add = DfsDinic(INT64_MAX, adjacency, from, to, layout)) {
            result += add;
        }
    }
    return -1;
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
    cout << graph::Dinic(edges, weights, 0, n - 1) << '\n';
}

int main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    Work();
}
