#include <iostream>
#include <vector>

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
private:
	std::vector<std::vector<uint32_t>> edges;
};

int dir[8][2] = {
	{ 2, 1 },
	{ 2, -1 },
	{ -2, -1 },
	{ -2, 1 },
	{ 1, -2 },
	{ 1, 2 },
	{ -1, -2 },
	{ -1, 2 }
};

bool suitable(int x) {
	for (int i = 0; i != 4; ++i) {
		if (x % 10 == 0) return false;
		x /= 10;
	}
	return true;
}

void work() {
	Directed_graph g(10000);
	for (int i = 1; i < 10000; ++i) if (suitable(i)) {
		if (i / 1000 < 9) g.add_link(i, i + 1000);
		if (i % 10 > 1) g.add_link(i, i - 1);
		g.add_link(i, i / 10 + 1000 * (i % 10));
		g.add_link(i, (i * 10) % 10000 + i / 1000);
	}
	int x1, x2;
	cin >> x1 >> x2;
	auto result = g.find_way(x1, x2);
	cout << result.size() << '\n';
	for (auto it : result) cout << it << '\n';
	cout << '\n';
}

int main() {
	cin.tie(0)->sync_with_stdio(0);
	work();
}
