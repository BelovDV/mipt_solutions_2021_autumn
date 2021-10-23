#include <vector>
#include <cstdint>

class Graph {
public:
	using Index = uint32_t;
	using Vertexes = std::vector<Index>;
	using Edges = std::vector<Vertexes>;
	using Colors = std::vector<uint8_t>;
public:
	Graph(size_t size) : edges(Edges(size)) {}
public: // DFS: unvisited - 0, visited - 1, in consideration - 2
	static void dfs(const Edges& edges, Colors& colors, Index root) {
		colors[root] = 1;
		for (auto next : edges[root])
			if (!colors[next])
				dfs(edges, colors, next);
	}
	static void dfs_order_tout(const Edges& edges, Colors& colors, Index root,
		Vertexes& order)
	{
		colors[root] = 1;
		for (auto next : edges[root])
			if (!colors[next])
				dfs_order_tout(edges, colors, next, order);
		order.push_back(root);
	}
	static void dfs_order_tin(const Edges& edges, Colors& colors, Index root,
		Vertexes& order)
	{
		order.push_back(root);
		colors[root] = 1;
		for (auto next : edges[root])
			if (!colors[next])
				dfs_order_tin(edges, colors, next, order);
	}
	static int64_t dfs_cycle(const Edges& edges, Colors& colors, Index root,
		Vertexes& parent)
	{
		colors[root] = 2;
		for (auto next : edges[root])
			if (colors[next] == 0) {
				parent[next] = root;
				auto result = dfs_cycle(edges, colors, next, parent);
				if (result != -1) return result;
			}
			else if (colors[next] == 2) {
				parent[next] = root;
				return next;
			}
		colors[root] = 1;
		return -1;
	}
public:
	Index size() const { return edges.size(); }
	Edges get_edges() const { return edges; }
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
		Colors visited(size());
		for (Index start = 0; start < size(); ++start)
			if (!visited[start])
				dfs_order_tout(edges, visited, start, order);
		visited.assign(size(), 0);
		auto reverse = get_edges_reverse();
		Vertexes component;
		std::vector<Vertexes> result;
		for (auto start = order.rbegin(); start != order.rend(); ++start)
			if (!visited[*start]) {
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
			if (!visited[start])
				dfs_order_tout(edges, visited, start, order);
		Vertexes result;
		visited.assign(size(), 0);
		for (auto start = order.rbegin(); start != order.rend(); ++ start)
			if (!visited[*start]) {
				dfs(edges, visited, *start);
				result.push_back(*start);
			}
		return result;
	}
	Vertexes get_cycle() const {
		Colors visited(size());
		Vertexes parent(size());
		auto reverse = get_edges_reverse();
		for (Index start = 0; start != size(); ++start) 
			if (!visited[start]) {
				auto res = dfs_cycle(reverse, visited, start, parent);
				if (res != -1) {
					Vertexes result;
					start = res;
					do {
						res = parent[res];
						result.push_back(res);
					} while (res != start);
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

int dir[][2] = {
	{0, 1},
	{0, -1},
	{1, 0},
	{-1, 0}
};

void work() {
	int n, m;
	cin >> n >> m;
	Graph g(n);
	while (m--) {
		int v1, v2;
		cin >> v1 >> v2;
		g.link(v1 - 1, v2 - 1);
	}
	/*for (int i = 0; i != n; ++i) {
		Graph::Colors visited(n);
		auto res = g.dfs_find_cycle_from(g.get_edges(), visited, i, i);
		if (res.size()) {
			cout << "YES\n";
			for (auto iter = res.rbegin(); iter != res.rend(); ++iter) {
				cout << 1 + *iter << ' ';
			}
			cout << "\n";
			return;
		}
	}*/
	auto res = g.get_cycle();
	if (res.size()) {
		cout << "YES\n";
		for (auto it : res) cout << it + 1 << ' ';
		cout << '\n';
	}
	else cout << "NO\n";
}

int main() {
	cin.tie(0)->sync_with_stdio(0);
	work();
}
