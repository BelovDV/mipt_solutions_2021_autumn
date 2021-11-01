#include <iostream>
#include <algorithm>
#include <vector>

using std::vector;

class Graph_q {
public:
	struct Direction {
		int to;
		int value;
	};
public:
	Graph_q(size_t sz) : edges(sz) {}
public:
	void link(int from, int to, int value) {
		edges[from].push_back({ to, value });
	}
	auto get_edges() const { return edges; }
public:
	struct Dfs {
		vector<char> x_value;
		vector<int> delta;
	};
	bool dfs(Dfs& str, int root, int& x) const {
		for (auto next_d : edges[root]) {
			auto next = next_d.to;
			if (str.x_value[next] == 0) {
				str.x_value[next] = str.x_value[root] ^ 3;
				str.delta[next] = next_d.value - str.delta[root];
				if (!dfs(str, next, x)) return false;
			}
			else if (str.x_value[next] == str.x_value[root]) {
				if (x == -1)
					x = (str.x_value[root] * 2 - 3) *
					((next_d.value - str.delta[root] - str.delta[next]) / 2);
				if (x * (str.x_value[root] * 2 - 3) * 2 + 
					str.delta[root] + str.delta[next] != next_d.value)
					return false;
			}
			else {
				if (str.delta[root] + str.delta[next] != next_d.value)
					return false;
			}
		}
		return true;
	}
public:
	vector<int> get_solution() const
	{
		Dfs str = {
			vector<char>(edges.size()), // 0-unvisited, 1 - -x, 2 - +x; value[i] * 2 - 3
			vector<int>(edges.size())
		};
		int x = -1;
		str.x_value[0] = 2;
		if (!dfs(str, 0, x))
			std::cout << "ERROR\n";
		if (x == -1) {
			int min_p = edges.size() * 5;
			int min_n = edges.size() * 5;
			for (int v = 0; v < edges.size(); ++v)
				if (str.x_value[v] == 1) min_n = std::min(min_n, str.delta[v]);
				else min_p = std::min(min_p, str.delta[v]);
			int x1 = 1 - min_p;
			int x2 = min_n - 1;

			vector<int> result_1(edges.size());
			for (int v = 0; v < edges.size(); ++v)
				result_1[v] = (str.x_value[v] * 2 - 3) * x1 + str.delta[v];

			std::sort(result_1.begin(), result_1.end());

			x = x1;
			for (int v = 0; v < edges.size(); ++v)
				if (result_1[v] != v + 1)
					x = x2;
		}
		vector<int> result(edges.size());
		for (int v = 0; v < edges.size(); ++v)
			result[v] = (str.x_value[v] * 2 - 3) * x + str.delta[v];
		return result;
	}
private:
	vector<vector<Direction>> edges;
};

using std::cin;
using std::cout;

void work() {
	int n, m;
	cin >> n >> m;
	Graph_q g(n);
	while (m--) {
		int f, t, w;
		cin >> f >> t >> w;
		g.link(f - 1, t - 1, w);
		g.link(t - 1, f - 1, w);
	}
	auto result = g.get_solution();
	cout << '\n';
	for (auto it : result) cout << it << ' ';
	cout << '\n';
}

int main() {
	cin.tie(0)->sync_with_stdio(0);

	//for (int i = 0; i < 100; ++i) test(i);
	work();
}
