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

int64_t DfsDinic(int64_t flow, vector<vector<int64_t>>& adj, size_t from, size_t to, vector<int64_t>& layout,
                 vector<vector<char>>& full) {
    if (flow == 0) {
        return 0;
    }
    if (from == to) {
        return flow;
    }
    for (size_t next = 0; next < adj.size(); ++next) {
        if (adj[from][next] <= 0 || layout[from] + 1 != layout[next] || full[from][next]) {
            continue;
        }
        int64_t try_flow = std::min(flow, adj[from][next]);
        int64_t pushed = DfsDinic(try_flow, adj, next, to, layout, full);
        if (pushed) {
            adj[from][next] -= pushed;
            adj[next][from] += pushed;
            return pushed;
        }
        full[from][next] = true;
    }
    return 0;
}

int64_t Dinic(const Edges& edges, const Weights& weights, size_t from, size_t to) {
    vector<vector<int64_t>> adjacency(edges.size(), vector<int64_t>(edges.size()));
    for (size_t from = 0; from < edges.size(); ++from) {
        for (size_t index = 0; index < edges[from].size(); ++index) {
            adjacency[from][edges[from][index]] += weights[from][index];
        }
    }
    int64_t result = 0;
    vector<std::pair<size_t, size_t>> stack;  // next, lay
    vector<int64_t> layout;
    while (true) {
        stack.clear();

        const int64_t zero = -2;
        stack.emplace_back(from, 0);
        size_t iter = 0;
        layout.assign(edges.size(), zero);
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

        vector<vector<char>> full(adjacency.size(), vector<char>(adjacency.size()));

        while (int64_t add = DfsDinic(INT64_MAX, adjacency, from, to, layout, full)) {
            result += add;
        }
    }
    return -1;
}

void LinkDirected(Edges& edges, Weights& weights, size_t from, size_t to, int64_t weight) {
    // cout << "from " << from << "\t to " << to << " \n";
    edges[from].push_back(to);
    // edges[to].push_back(from);
    weights[from].push_back(weight);
    // weights[to].push_back(weight);
}

}  // namespace graph

void Work() {
    size_t n = 0;
    size_t m = 0;
    int64_t need_to_result_from = 0;
    int64_t need_to_result_to = 0;
    cin >> n >> m;
    graph::Edges edges(n * m + 2);
    graph::Weights weights(n * m + 2);
    for (size_t y = 0; y < n; ++y) {
        for (size_t x = 0; x < m; ++x) {
            // cout << "next\n";
            size_t from = 0;
            size_t to = 1;
            int weight = 0;
            char c = 0;
            cin >> c;
            if (c == 'H') {
                weight = 1;
            } else if (c == 'O') {
                weight = 2;
            } else if (c == 'N') {
                weight = 3;
            } else if (c == 'C') {
                weight = 4;
            }
            if ((x + y) % 2) {
                to = y * m + x + 2;
                // cout << "to " << y << ' ' << x << '\t' << to << '\n';
                size_t cur = y * m + x + 2;
                if (x > 0) {
                    graph::LinkDirected(edges, weights, cur, cur - 1, 1);
                }
                if (x < m - 1) {
                    graph::LinkDirected(edges, weights, cur, cur + 1, 1);
                }
                if (y > 0) {
                    graph::LinkDirected(edges, weights, cur, cur - m, 1);
                }
                if (y < n - 1) {
                    graph::LinkDirected(edges, weights, cur, cur + m, 1);
                }
                need_to_result_from += weight;
            } else {
                need_to_result_to += weight;
                from = y * m + x + 2;
            }

            graph::LinkDirected(edges, weights, from, to, weight);
        }
    }

    // for (size_t from = 0; from < edges.size(); ++from) {
    //     cout << "from: " << from << ": \t";
    //     for (size_t index = 0; index < edges[from].size(); ++index) {
    //         cout << edges[from][index] << " (" << weights[from][index] << ")\t";
    //     }
    //     cout << '\n';
    // }
    // cout << "need " << need_to_result_from << '\n';
    cout << '\n';

    if (need_to_result_to != need_to_result_from || need_to_result_from == 0) {
        cout << "Invalid\n";
        return;
    }

    if (graph::Dinic(edges, weights, 0, 1) == need_to_result_from && need_to_result_from == need_to_result_to &&
        need_to_result_to > 0) {
        cout << "Valid\n";
    } else {
        cout << "Invalid\n";
    }
}

int main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    Work();
}
