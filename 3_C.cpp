#include <iostream>
#include <vector>
#include <cstdint>

#include <set>

using std::cin;
using std::cout;
using std::vector;

class Graph {
    using Weight = int64_t;
    using Index = size_t;

public:
    explicit Graph(size_t size) : edges_(size), weights_(size) {
    }

public:
    void Link(Index from, Index to, int64_t weight) {
        edges_[from].push_back(to);
        weights_[from].push_back(weight);
    }
    Weight MinimalSpanningTree() const {
        std::set<std::pair<Weight, std::pair<Index, Index>>> set;
        // weight ; (from, index in edges[from])
        auto f_weight = &std::pair<Weight, std::pair<Index, Index>>::first;
        auto edge = &std::pair<Weight, std::pair<Index, Index>>::second;
        auto f_from = &std::pair<Index, Index>::first;
        auto f_index = &std::pair<Index, Index>::second;
        for (size_t index = 0; index < edges_[0].size(); ++index) {
            set.insert({weights_[0][index], {0, index}});
        }

        vector<Index> visited(edges_.size());
        visited[0] = true;

        Weight result = 0;

        size_t without_goto = 1;
        while (without_goto < edges_.size()) {
            auto iter = set.begin();
            size_t next = edges_[*iter.*edge.*f_from][*iter.*edge.*f_index];

            if (visited[next]) {
                set.erase(iter);
                continue;
            }
            ++without_goto;
            visited[next] = true;
            result += *iter.*f_weight;
            set.erase(iter);

            for (size_t index = 0; index < edges_[next].size(); ++index) {
                set.insert({weights_[next][index], {next, index}});
            }
        }
        return result;
    }

private:
    vector<vector<Index>> edges_;
    vector<vector<Weight>> weights_;
};

void Work() {
    int vertex_count = 0;
    int edges_count = 0;
    cin >> vertex_count >> edges_count;
    Graph g(vertex_count);
    while (edges_count--) {
        int from = 0;
        int to = 0;
        int weight = 0;
        cin >> from >> to >> weight;
        g.Link(from - 1, to - 1, weight);
        g.Link(to - 1, from - 1, weight);
    }

    cout << g.MinimalSpanningTree() << '\n';
}

int main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    Work();
}
