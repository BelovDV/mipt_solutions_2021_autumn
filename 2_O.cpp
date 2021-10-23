#include <vector>
#include <cstdint>
#include <algorithm>

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
	static void dfs_paint(const Edges& edges, std::vector<int64_t>& colors,
		Index root, int64_t color) {
		colors[root] = color;
		for (auto next : edges[root])
			if (!colors[next])
				dfs_paint(edges, colors, next, color);
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
	void link_reverse(Index from, Index to) {
		edges[to].push_back(from);
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
		for (auto start = order.rbegin(); start != order.rend(); ++start)
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
	Vertexes get_cycle_reverse() const {
		Colors visited(size());
		Vertexes parent(size());
		for (Index start = 0; start != size(); ++start)
			if (!visited[start]) {
				auto res = dfs_cycle(edges, visited, start, parent);
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
	Vertexes get_path(Index from, Index to) const {
		auto reverse = get_edges_reverse();
		Vertexes queue(1, to);
		Vertexes parent(size(), from);
		for (Index iter = 0; iter < queue.size(); ++iter)
			for (auto next : reverse[queue[iter]])
				if (next == from) {
					parent[next] = queue[iter];
					Vertexes result;
					while (next != to) result.push_back(next = parent[next]);
					return result;
				}
				else if (parent[next] == from) {
					parent[next] = queue[iter];
					queue.push_back(next);
				}
		return Vertexes();
	}
	Vertexes get_accessible(Index from) const {
		Colors visited(size());
		dfs(edges, visited, from);
		Vertexes result;
		for (Index i = 0; i < size(); ++i)
			if (visited[i])
				result.push_back(i);
		return result;
	}
	std::vector<Vertexes> undirected_get_connections_component() const
	{
		std::vector<int64_t> colors(size());
		std::vector<int64_t> possible_colors;
		for (Index start = 0; start != size(); ++start)
			if (!colors[start]) {
				dfs_paint(edges, colors, start, (int64_t)start + 1);
				possible_colors.push_back((int64_t)start + 1);
			}
		std::vector<Vertexes> result(possible_colors.size());
		for (Index start = 0; start != size(); ++start) {
			auto iter = std::lower_bound(
				possible_colors.begin(),
				possible_colors.end(),
				colors[start]);
			result[iter - possible_colors.begin()].push_back(start);
		}
		return result;
	}
	Vertexes acyclic_get_top_sort() const {
		Vertexes order;
		Colors visited(size());
		for (Index start = 0; start < size(); ++start)
			if (!visited[start])
				dfs_order_tout(edges, visited, start, order);
		std::reverse(order.begin(), order.end());
		return order;
	}
private:
	Edges edges;
};

class Graph_weightened : public Graph {
public:
	using Weight = uint8_t;
	using Weights = std::vector<Weight>;
	using Edges_weights = std::vector<Weights>;
	const Weight weight_max = uint8_t(0xFF);
public:
	Graph_weightened(size_t size) :
		Graph(size),
		weights(Edges_weights(size))
	{}
public:
	void link(Index from, Index to, Weight weight) {
		Graph::link(from, to);
		weights[from].push_back(weight);
	}
	int64_t get_distance(Index from, Index to) const {
		std::vector<uint8_t> visited(size());
		std::vector<Vertexes> levels_v(weight_max);
		const Edges& edges = Graph::get_edges();
		levels_v[0].push_back(from);
		Index remain = 1;
		for (Index answer = 0; remain > 0; ++answer) {
			size_t level = answer % weight_max;
			size_t cur_ind = 0;
			for (; cur_ind != levels_v[level].size(); ++cur_ind) {
				Index cur = levels_v[level][cur_ind];
				--remain;
				if (cur == to) return answer;
				if (visited[cur]) continue;
				visited[cur] = true;
				for (Index n_ind = 0; n_ind < edges[cur].size(); ++n_ind) {
					Index dest = edges[cur][n_ind];
					if (visited[dest]) continue;
					Weight weight = weights[cur][n_ind];
					++remain;
					levels_v[(level + weight) % weight_max].push_back(dest);
				}
			}
			levels_v[level].clear();
		}
		return -1;
	}
private:
	Edges_weights weights;
};

#include <iostream>

using std::cin;
using std::cout;
using std::vector;

void work() {
	int n;
	cin >> n;
	Graph g(4 * n);
	for (int from = 0; from < n - 1; ++from) {
		std::string input;
		cin >> input;
		for (int delta = 0; delta < input.size(); ++delta) {
			int to = from + delta + 1;
			if (input[delta] == 'R') {
				g.link_reverse(from, to);
				g.link_reverse(from, to + n);
			}
			else {
				g.link_reverse(2 * n + to, 3 * n + from);
				g.link_reverse(3 * n + to, 3 * n + from);
			}
		}
	}
	for (int i = 0; i < n; ++i) {
		g.link_reverse(n + i, 2 * n + i);
		g.link_reverse(3 * n + i, i);
	}
	if (g.get_cycle_reverse().size())
		cout << "NO\n";
	else
		cout << "YES\n";
}

int main() {
	cin.tie(0)->sync_with_stdio(0);
	work();
}
