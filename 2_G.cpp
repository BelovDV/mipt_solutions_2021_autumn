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
public: // DFS 
		// i visited <=> colors[i] & 1
	static void dfs(const Edges& edges, Colors& colors, Index root) {
		colors[root] |= 1;
		for (auto next : edges[root])
			if (!(colors[next] & 1))
				dfs(edges, colors, next);
	}
	static void dfs_order_tout(const Edges& edges, Colors& colors, Index root,
		Vertexes& order)
	{
		colors[root] |= 1;
		for (auto next : edges[root])
			if (!(colors[next] & 1))
				dfs(edges, colors, next);
		order.push_back(root);
	}
	static void dfs_order_tin(const Edges& edges, Colors& colors, Index root,
		Vertexes& order)
	{
		order.push_back(root);
		colors[root] |= 1;
		for (auto next : edges[root])
			if (!(colors[next] & 1))
				dfs(edges, colors, next);
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
	std::vector<Vertexes> compress() const {
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
	int h, w;
	cin >> h >> w;
	Graph g(h * w);
	vector<int> input(w * h);
	for (auto& it : input) cin >> it;
	for (int y = 0; y < h; ++y) for (int x = 0; x < w; ++x) {
		int v = y * w + x;
		for (int d = 0; d != 4; ++d) {
			int nx = x + dir[d][0], ny = y + dir[d][1], nv = ny * w + nx;
			if (nx >= 0 && nx < w && ny >= 0 && ny < h) {
				if (input[v] <= input[nv]) g.link(v, nv);
				if (input[v] >= input[nv]) g.link(nv, v);
			}
		}
	}
	cout << g.get_control_set().size() << '\n';
}

int main() {
	cin.tie(0)->sync_with_stdio(0);
	work();
}
