#include <iostream>
#include <vector>
#include <queue>

using std::cin;
using std::cout;
using std::vector;

class Graph {
public:
    using Index = size_t;

public:
    Graph(size_t size) : edges(std::vector<std::vector<Index>>(size)) {
    }

public:
    const std::vector<Index>& neighbors(Index vertex) const {
        return edges[vertex];
    }
    void add_link(Index vertex_1, Index vertex_2) {
        edges[vertex_1].push_back(vertex_2);
        edges[vertex_2].push_back(vertex_1);
    }
    std::vector<Index> find_shortest_way(Index from, Index to) {
        std::vector<int64_t> parent(edges.size(), -1);
        parent[to] = to;

        std::queue<int64_t> queue;
        queue.push(to);
        while (!queue.empty()) {
            auto next_root = queue.front();
            queue.pop();
            for (auto next : edges[next_root]) {
                if (next == from) {
                    parent[next] = next_root;
                    std::vector<Index> result(1, next);
                    while (next != to)
                        result.push_back(next = parent[next]);
                    return result;
                } else if (parent[next] == -1) {
                    parent[next] = next_root;
                    queue.push(next);
                }
            }
        }
        return std::vector<Index>();
    }
    bool is_bipartite() const {
        std::vector<int> color(edges.size());
        std::vector<int> stack;
        for (size_t i = 0; i < edges.size(); ++i) {
            if (color[i] == 0) {
                stack.push_back(i);
                color[i] = 1;
                while (stack.size()) {
                    auto work = stack.back();
                    stack.pop_back();
                    for (auto next : edges[work]) {
                        if (color[next] == 0) {
                            color[next] = 3 ^ color[work];
                            stack.push_back(next);
                        } else if (color[next] == color[work])
                            return false;
                    }
                }
            }
        }
        return true;
    }

private:
    std::vector<std::vector<Index>> edges;
};

void work() {
    int vertexes_count, edges_count, from, to;
    cin >> vertexes_count >> edges_count >> from >> to;
    Graph g(vertexes_count);
    while (edges_count--) {
        int vsp_from, vsp_to;
        cin >> vsp_from >> vsp_to;
        g.add_link(vsp_from - 1, vsp_to - 1);
    }
    auto shortest_way = g.find_shortest_way(from - 1, to - 1);
    if (shortest_way.size()) {
        cout << shortest_way.size() - 1 << '\n';
        for (auto it : shortest_way)
            cout << it + 1 << ' ';
        cout << '\n';
    } else
        cout << "-1\n";
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    work();
}
