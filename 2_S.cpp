#include <iostream>
#include <algorithm>
#include <vector>

using std::vector;

class Graph_s {
public:
	struct Direction {
		int to;
		int value;
	};
public:
	Graph_s(size_t sz) : edges(sz) {}
public:
	void link(int from, int to, int value) {
		edges[from].push_back({ to, value });
		edges[to].push_back({ from, value });
	}
public:
	struct Dfs {
		vector<char> color;
		vector<int> height;
		vector<int> result;
	};
	int dfs(Dfs& str, int root, int h) const {
		str.color[root] = true;
		str.height[root] = h;
		int upper = h;
		int parent_edges = 0;
		for (auto next : edges[root])
			if (str.color[next.to]) {
				if (str.height[next.to] == h - 1) {
					++parent_edges;
					if (parent_edges == 2)
						upper = std::min(upper, str.height[next.to]);
				}
				else
					upper = std::min(upper, str.height[next.to]);
			}
			else {
				int next_upper = dfs(str, next.to, h + 1);
				if (next_upper == h + 1)
					str.result.push_back(next.value);
				upper = std::min(upper, next_upper);
			}
		return upper;
	}
public:
	vector<int> get_bridges() const {
		Dfs str = {
			vector<char>(edges.size()),
			vector<int>(edges.size(), -1),
			vector<int>() 
		};
		for (int root = 0; root < edges.size(); ++root)
			if (!str.color[root])
				dfs(str, root, 1);
		return str.result;
	}
private:
	vector<vector<Direction>> edges;
};

using std::cin;
using std::cout;

int main() {
	cin.tie(0)->sync_with_stdio(0);

	int n, m;
	cin >> n >> m;

	Graph_s g(n);

	for (int i = 1; i <= m; ++i) {
		int f, t;
		cin >> f >> t;
		g.link(f - 1, t - 1, i);
	}

	auto result = g.get_bridges();
	
	std::sort(result.begin(), result.end());
	cout << result.size() << '\n';
	for (auto it : result) cout << it << ' ';
	cout << '\n';
}
