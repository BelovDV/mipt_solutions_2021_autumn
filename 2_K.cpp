#include <vector>
#include <cstdint>

class Graph {
public:
    using Index = uint32_t;
    using Vertexes = std::vector<Index>;
    using Edges = std::vector<Vertexes>;
    enum class Color { white, black, gray };
    using Colors = std::vector<Color>;

public:
    Graph(size_t size) : edges(Edges(size)) {
    }

public:  // DFS: unvisited - 0, visited - 1, in consideration - 2
    static void dfs(const Edges& edges, Colors& colors, Index root) {
        colors[root] = Color::black;
        for (auto next : edges[root])
            if (colors[next] != Color::white)
                dfs(edges, colors, next);
    }
    static void dfs_order_tout(const Edges& edges, Colors& colors, Index root, Vertexes& order) {
        colors[root] = Color::black;
        for (auto next : edges[root])
            if (colors[next] != Color::white)
                dfs_order_tout(edges, colors, next, order);
        order.push_back(root);
    }
    static void dfs_order_tin(const Edges& edges, Colors& colors, Index root, Vertexes& order) {
        order.push_back(root);
        colors[root] = Color::black;
        for (auto next : edges[root])
            if (colors[next] != Color::white)
                dfs_order_tin(edges, colors, next, order);
    }
    static int64_t dfs_cycle(const Edges& edges, Colors& colors, Index root, Vertexes& parent) {
        colors[root] = Color::gray;
        for (auto next : edges[root]) {
            parent[next] = root;
            if (colors[next] == Color::white) {
                auto expand_start = dfs_cycle(edges, colors, next, parent);
                if (expand_start != -1)
                    return expand_start;
            } else if (colors[next] == Color::gray)
                return next;
        }
        colors[root] = Color::black;
        return -1;
    }

public:
    Index size() const {
        return edges.size();
    }
    Edges get_edges() const {
        return edges;
    }
    Edges get_edges_reverse() const {
        Edges result(size());
        for (Index ver = 0; ver < size(); ++ver)
            for (auto next : edges[ver])
                result[next].push_back(ver);
        return result;
    }
    void link(Index from, Index to) {
        edges[from].push_back(to);
    }

public:
    std::vector<Vertexes> get_compressed() const {
        Vertexes order;
        Colors visited(size(), Color::white);
        for (Index start = 0; start < size(); ++start)
            if (visited[start] == Color::white)
                dfs_order_tout(edges, visited, start, order);
        visited.assign(size(), Color::white);
        auto reverse = get_edges_reverse();
        Vertexes component;
        std::vector<Vertexes> result;
        for (auto start = order.rbegin(); start != order.rend(); ++start)
            if (visited[*start] == Color::white) {
                component.clear();
                dfs_order_tout(reverse, visited, *start, component);
                result.push_back(component);
                component.clear();
            }
        return result;
    }
    Vertexes get_control_set() const {
        Vertexes order;
        Colors visited(size());
        for (Index start = 0; start < size(); ++start)
            if (visited[start] == Color::white)
                dfs_order_tout(edges, visited, start, order);
        Vertexes result;
        visited.assign(size(), Color::white);
        for (auto start = order.rbegin(); start != order.rend(); ++start)
            if (visited[*start] == Color::white) {
                dfs(edges, visited, *start);
                result.push_back(*start);
            }
        return result;
    }
    Vertexes get_cycle() const {
        Colors visited(size(), Color::white);
        Vertexes parent(size());
        auto reverse = get_edges_reverse();
        for (Index start = 0; start != size(); ++start)
            if (visited[start] == Color::white) {
                auto expand_start = dfs_cycle(reverse, visited, start, parent);
                if (expand_start != -1) {
                    Vertexes result;
                    start = expand_start;
                    do {
                        expand_start = parent[expand_start];
                        result.push_back(expand_start);
                    } while (expand_start != start);
                    return result;
                }
            }
        return Vertexes();
    }

private:
    Edges edges;
};

#include <iostream>

using std::cin;
using std::cout;
using std::vector;

void work() {
    int count_vertexes, count_edges;
    cin >> count_vertexes >> count_edges;
    Graph g(count_vertexes);
    while (count_edges--) {
        int from, to;
        cin >> from >> to;
        g.link(from - 1, to - 1);
    }
    auto res = g.get_cycle();
    if (res.size()) {
        cout << "YES\n";
        for (auto it : res)
            cout << it + 1 << ' ';
        cout << '\n';
    } else
        cout << "NO\n";
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    work();
}
