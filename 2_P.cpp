#include <vector>
#include <cstdint>
#include <algorithm>
#include <unordered_set>

class Graph {
public:
    using Index = uint32_t;
    using Vertexes = std::vector<Index>;
    using Edges = std::vector<Vertexes>;
    enum class Color { white, black };
    using Colors = std::vector<Color>;

public:
    Graph(size_t size) : edges(size) {
    }
    Graph(Edges&& edges) : edges(std::move(edges)) {
    }

public:
    template <typename Edges_list>
    static void dfs_order_tout(const Edges_list& edges, Colors& colors, Index root, Vertexes& order) {
        colors[root] = Color::black;
        for (auto next : edges[root])
            if (colors[next] == Color::white)
                dfs_order_tout(edges, colors, next, order);
        order.push_back(root);
    }

public:
    Index size() const {
        return edges.size();
    }
    Edges get_edges() const {
        return edges;
    }
    Graph get_reverse() const {
        Edges result(size());
        for (Index ver = 0; ver < size(); ++ver)
            for (auto next : edges[ver])
                result[next].push_back(ver);
        return Graph(std::move(result));
    }
    void link(Index f, Index t) {
        edges[f].push_back(t);
    }

public:
    std::vector<Index> get_compressed_sorted() const {
        Vertexes order;
        Colors visited(size(), Color::white);
        for (Index start = 0; start < size(); ++start)
            if (visited[start] == Color::white)
                dfs_order_tout(edges, visited, start, order);
        auto reversed = get_reverse();
        visited.assign(size(), Color::white);
        std::vector<std::vector<Index>> components;
        for (auto start = order.rbegin(); start != order.rend(); ++start)
            if (visited[*start] == Color::white) {
                components.push_back(std::vector<Index>());
                dfs_order_tout(reversed.get_edges(), visited, *start, components.back());
            }
        std::vector<Index> component_index(size());
        for (size_t i = 0; i < components.size(); ++i)
            for (auto element : components[i])
                component_index[element] = i;
        return component_index;
    }

private:
    Edges edges;
};

#include <iostream>

using std::cin;
using std::cout;
using std::vector;

void work() {
    int n, m;
    cin >> n >> m;
    Graph g(n);
    while (m--) {
        int f, t;
        cin >> f >> t;
        g.link(f - 1, t - 1);
    }

    auto result = g.get_compressed_sorted();
    Graph::Index maximum = 0;
    for (auto it : result)
        maximum = std::max(maximum, it);
    cout << maximum + 1 << '\n';
    for (auto it : result)
        cout << it + 1 << ' ';
    cout << '\n';
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    work();
}