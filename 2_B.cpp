#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

class Graph {
public:
    enum Color { white = 1, gray = 2 };

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
        parent[from] = from;
        std::vector<uint32_t> queue(edges.size(), from);
        auto current = queue.begin();
        auto end = current + 1;
        while (current != end) {
            for (auto next : edges[*current])
                if (parent[next] == -1) {
                    parent[next] = *current;
                    *(end++) = next;
                    if (next == to) {
                        std::vector<uint32_t> result;
                        while (next != from) {
                            result.push_back(next);
                            next = parent[next];
                        }
                        result.push_back(next);
                        return result;
                    }
                }
            ++current;
        }
        return std::vector<uint32_t>();
    }
    bool is_bipartite() const {
        std::vector<Color> color(edges.size());
        std::vector<int> stack;
        for (size_t i = 0; i < edges.size(); ++i) {
            if (color[i] == 0) {
                stack.push_back(i);
                color[i] = Color::white;
                while (stack.size()) {
                    auto work = stack.back();
                    stack.pop_back();
                    for (auto next : edges[work]) {
                        if (color[next] == 0) {
                            color[next] = color[work] == Color::white ? Color::gray : Color::white;
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

#include <unordered_map>
#include <unordered_set>
#include <set>

void test() {
    int n = 10000, m = 1000;
    std::set<std::pair<int, int>> map;
    for (int i = 0; i != m; ++i) {
        int a = rand() % n, b = rand() % n;
        map.insert({a, b});
        map.insert({b, a});
    }
    map.insert({0, 10});
    map.insert({10, 0});
    map.insert({50, 10});
    map.insert({10, 50});
    map.insert({50, n - 1});
    map.insert({n - 1, 50});
    Graph g(n);
    for (auto& it : map)
        g.add_link(it.first, it.second);
    auto result = g.find_way(0, n - 1);
    if (result.size() > 4) {
        cout << "FIND\n";
    }
    for (size_t i = 1; i < result.size(); ++i) {
        if (map.find({result[i], result[i - 1]}) == map.end()) {
            cout << "FIND\n";
        } else {
            // cout << "DONE\n";
        }
    }
}

void work() {
    int n, m;
    cin >> n >> m;
    Graph g(n);
    for (int i = 0; i != m; ++i) {
        int v1, v2;
        cin >> v1 >> v2;
        g.add_link(v1 - 1, v2 - 1);
    }
    if (g.is_bipartite())
        cout << "YES\n";
    else
        cout << "NO\n";
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    work();
}
