#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

class Graph {
public:
	Graph(uint32_t size) : 
		edges(std::vector<std::vector<uint32_t>>(size)) 
	{}
	Graph(std::vector<std::vector<uint32_t>>&& source) : 
		edges(std::move(source)) 
	{}
public:
	const std::vector<uint32_t>& neighbours(uint32_t vertex) const { 
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
				}
				else if (parent[next] == -1) {
					parent[next] = queue[i];
					queue.push_back(next);
					if (queue.size() > edges.size())while (1) {}
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
						}
						else if (color[next] == color[work])
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

void work() {
	int n, m, a, b;
	cin >> n >> m >> a >> b;
	Graph g(n);
	while (m--) {
		int v1, v2;
		cin >> v1 >> v2;
		g.add_link(v1 - 1, v2 - 1);
	}
	auto result = g.find_way(a - 1, b - 1);
	if (result.size()) {
		cout << result.size() - 1 << '\n';
		for (auto it : result) cout << it + 1 << ' ';
		cout << '\n';
	}
	else cout << "-1\n";
}

int main() {
	cin.tie(0)->sync_with_stdio(0);
	work();
}
