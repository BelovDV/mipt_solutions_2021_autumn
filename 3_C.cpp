#include <iostream>
#include <vector>
#include <cstdint>

#include <set>

using std::cin;
using std::cout;
using std::vector;

namespace graph {
int64_t MinimalSpanningTree(const vector<vector<size_t>>& edges, const vector<vector<int64_t>>& weights) {
    std::set<std::pair<int64_t, std::pair<size_t, size_t>>> set;
    // weight ; (from, index in edges[from])
    for (size_t index = 0; index < edges[0].size(); ++index) {
        set.insert({weights[0][index], {0, index}});
    }

    vector<size_t> visited(edges.size());
    visited[0] = true;

    int64_t result = 0;

    size_t without_goto = 1;
    while (without_goto < edges.size()) {
        auto iter = set.begin();
        size_t next = edges[iter->second.first][iter->second.second];

        if (visited[next]) {
            set.erase(iter);
            continue;
        }
        // cout << iter->second.first << ' ' << next << '\n';
        ++without_goto;
        visited[next] = true;
        result += iter->first;
        set.erase(iter);

        for (size_t index = 0; index < edges[next].size(); ++index) {
            set.insert({weights[next][index], {next, index}});
        }
    }
    return result;
}
}  // namespace graph

void Work() {
    int n = 0;
    int m = 0;
    cin >> n >> m;
    vector<vector<size_t>> edges(n);
    vector<vector<int64_t>> weights(n);
    while (m--) {
        int from = 0;
        int to = 0;
        int weight = 0;
        cin >> from >> to >> weight;
        edges[from - 1].push_back(to - 1);
        edges[to - 1].push_back(from - 1);
        weights[from - 1].push_back(weight);
        weights[to - 1].push_back(weight);
    }

    cout << graph::MinimalSpanningTree(edges, weights) << '\n';
}

int main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    Work();
}
