#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

class Graph {
public:
    Graph(uint32_t size) : edges(std::vector<std::vector<uint32_t>>(size)) {
    }
    Graph(std::vector<std::vector<uint32_t>>&& source) : edges(std::move(source)) {
    }

public:
    const std::vector<uint32_t>& neighbors(uint32_t vertex) const {
        return edges[vertex];
    }
    void add_link(uint32_t vertex_1, uint32_t vertex_2) {
        edges[vertex_1].push_back(vertex_2);
        edges[vertex_2].push_back(vertex_1);
    }
    std::vector<uint32_t> find_way(uint32_t from, uint32_t to) {
        std::vector<int64_t> parent(edges.size(), -1);
        parent[to] = to;
        std::vector<int64_t> queue(1, to);
        for (int i = 0; i < queue.size(); ++i) {
            for (auto next : edges[queue[i]]) {
                if (next == from) {
                    parent[next] = queue[i];
                    std::vector<uint32_t> result(1, next);
                    while (next != to)
                        result.push_back(next = parent[next]);
                    return result;
                } else if (parent[next] == -1) {
                    parent[next] = queue[i];
                    queue.push_back(next);
                    if (queue.size() > edges.size())
                        while (1) {
                        }
                }
            }
        }
        return std::vector<uint32_t>();
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
    std::vector<std::vector<uint32_t>> edges;
};

struct Vector {
    int x;
    int y;
    void operator+=(const Vector& summand) {
        x += summand.x;
        y += summand.y;
    }
};

Vector direction[8] = {Vector({2, 1}),  Vector({2, -1}), Vector({-2, -1}), Vector({-2, 1}),
                       Vector({1, -2}), Vector({1, 2}),  Vector({-1, -2}), Vector({-1, 2})};

struct Rect {
    int left, right, top, bottom;  // if right < left - square := 0
    bool contains(const Vector& vec) {
        return vec.x >= left && vec.x < right && vec.y >= bottom && vec.y < top;
    }
};

void work() {
    int n;
    cin >> n;
    Graph g(n * n);
    Rect grid({0, 0, n, n});
    for (int i = 0; i < n * n; ++i) {
        for (int d = 0; d != 8; ++d) {
            Vector pos({i % n, i / n});
            pos += direction[d];
            if (grid.contains(pos))
                g.add_link(i, pos.y * n + pos.x);
        }
    }
    int x1, x2, y1, y2;
    cin >> y1 >> x1 >> y2 >> x2;
    auto result = g.find_way(x1 + y1 * n - n - 1, x2 + y2 * n - n - 1);
    cout << result.size() - 1 << '\n';
    for (auto& it : result) {
        cout << it / n + 1 << ' ' << it % n + 1 << '\n';
    }
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    work();
}
