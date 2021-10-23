#include <iostream>
#include <vector>
#include <unordered_set>

using std::cin;
using std::cout;
using std::vector;

class Directed_graph {
public:
	Directed_graph(uint32_t size) :
		edges(std::vector<std::vector<uint32_t>>(size)) 
	{}
	Directed_graph(std::vector<std::vector<uint32_t>>&& source) :
		edges(std::move(source)) 
	{}
public:
	const std::vector<uint32_t>& neighbours(uint32_t vertex) const { 
		return edges[vertex]; 
	}
	void add_link(uint32_t vertex_1, uint32_t vertex_2) {
		edges[vertex_1].push_back(vertex_2);
		//edges[vertex_2].push_back(vertex_1);
	}
	void add_link_both(uint32_t vertex_1, uint32_t vertex_2) {
		edges[vertex_1].push_back(vertex_2);
		edges[vertex_2].push_back(vertex_1);
	}
	std::vector<uint32_t> find_way(uint32_t from, uint32_t to) {
		std::vector<int64_t> parent(edges.size(), -1);
		parent[from] = from;
		std::vector<int64_t> queue(1, from);
		for (int i = 0; i < queue.size(); ++i) {
			for (auto next : edges[queue[i]]) {
				if (next == to) {
					parent[next] = queue[i];
					std::vector<uint32_t> vsp(1, next);
					while (next != from)
						vsp.push_back(next = parent[next]);
					std::vector<uint32_t> result(vsp.size());
					for (size_t i = 0; i != vsp.size(); ++i)
						result[result.size() - i - 1] = vsp[i];
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
	int64_t find_dist(uint32_t from, const std::unordered_set<uint32_t>& to) {
		if (to.count(from)) return 0;
		std::vector<int64_t> parent(edges.size(), -1);
		parent[from] = from;
		std::vector<int64_t> queue(1, from);
		for (int i = 0; i < queue.size(); ++i) {
			for (auto next : edges[queue[i]]) {
				if (to.count(next)) {
					parent[next] = queue[i];
					uint32_t result = 0;
					while (next != from) {
						++result;
						next = parent[next];
					}
					return result;
				}
				else if (parent[next] == -1) {
					parent[next] = queue[i];
					queue.push_back(next);
					if (queue.size() > edges.size())while (1) {}
				}
			}
		}
		return -1;
	}
private:
	std::vector<std::vector<uint32_t>> edges;
};

int dir[][2] = {
	{1,0},
	{-1,0},
	{0,1},
	{0,-1}
}; 

void work() {
	int m, n;
	cin >> m >> n;
	Directed_graph g(m * n);
	vector<char> input(m * n);
	int s = -1, t = -1;
	for (auto& it : input) cin >> it;
	for (int i = 0; i < m * n; ++i) {
		if (input[i] == '#') continue;
		if (input[i] == 'S') s = i;
		if (input[i] == 'T') t = i;
		for (int d = 0; d < 4; ++d) {
			int x = i % n, y = i / n;
			int dx = 0, dy = 0;
			while (
				x + dx < n && y + dy < m && 
				x + dx >= 0 && y + dy >= 0 && 
				input[x + dx + n * (y + dy)] != '#'
				) {
				dx += dir[d][0];
				dy += dir[d][1];
			}
			if (dx > 1 || dx < -1 || dy > 1 || dy < -1)
				g.add_link(i, i + dx / 2 + (dy / 2) * n);
		}
	}
	cout << (int64_t)g.find_way(s, t).size() - 1 << '\n';
}

int main() {
	cin.tie(0)->sync_with_stdio(0);
	work();
}
