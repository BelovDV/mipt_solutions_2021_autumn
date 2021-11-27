#include <vector>
#include <cstdint>
#include <algorithm>
#include <functional>

class Graph {
public:
	using Index = uint32_t;
	using Vertexes = std::vector<Index>;
	using Edges = std::vector<Vertexes>;
	using Colors = std::vector<uint8_t>;
public:
	Graph(size_t size) : edges(Edges(size)) {}
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
	static void dfs_order_tout(const Edges& edges, Colors& colors, Index root,
		Vertexes& order)
	{
		colors[root] = 1;
		for (auto next : edges[root])
			if (!colors[next])
				dfs_order_tout(edges, colors, next, order);
		order.push_back(root);
	}
public:
	static uint32_t dfs_keeping_points(
		const Edges& edges, Colors& colors, Index root,
		std::vector<uint32_t>& height, uint32_t h, Vertexes& result
	)
	{
		colors[root] = 1;
		height[root] = h;
		uint32_t r_value = h;
		uint32_t up_from_down = h;
		uint32_t under = 0;
		bool is_result = false;
		for (auto next : edges[root])
			if (colors[next])
				r_value = std::min(r_value, height[next]);
			else {
				++under;
				auto up_from_next = dfs_keeping_points(edges, colors, next, height, h + 1, result);
				up_from_down = std::min(up_from_down, up_from_next);
				if (up_from_next == h)
					is_result = true;
			}
		if (h == 1) {
			if (under > 1)
				result.push_back(root);
		}
		else
			if (is_result)
				result.push_back(root);
		return std::min(up_from_down, r_value);
	}
	Vertexes undirected_get_keeping_points() const
	{
		Colors colors(size());
		std::vector<uint32_t> heights(size());
		Vertexes result;
		for (Index root = 0; root < size(); ++root)
			if (!colors[root])
				dfs_keeping_points(edges, colors, root, heights, 1, result);
		return result;
	}
private:
	Edges edges;
};

#include <iostream>
#include <unordered_set>

using std::cin;
using std::cout;
using std::vector;

void work() {
	int n, m;
	cin >> n >> m;
	Graph g(m);
	vector<vector<int>> touched(n);
	for (int i = 0; i < m; ++i) {
		int a, b, c;
		cin >> a >> b >> c;
		touched[a - 1].push_back(i);
		touched[b - 1].push_back(i);
		touched[c - 1].push_back(i);
	}
	for (int i = 0; i < n; ++i) {
		if (touched[i].size() > 1) {
			for (int from = 0; from < touched[i].size() - 1; ++from) {
				g.link(touched[i][from], touched[i][from + 1]);
				g.link(touched[i][from + 1], touched[i][from]);
			}
			g.link(touched[i].front(), touched[i].back());
			g.link(touched[i].back(), touched[i].front());
		}
	}
	auto res = g.undirected_get_keeping_points();

	std::unordered_set<int> result;
	for (auto it : res) result.insert(it);

	for (auto it : touched)
		if (it.size() == 1)
			result.insert(it[0]);

	cout << result.size();
	cout << '\n';
	for (auto it : result)
		cout << it + 1 << ' ';
	cout << '\n';
}

int main() {
	cin.tie(0)->sync_with_stdio(0);
	work();
}
