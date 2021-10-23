#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

template <typename T>
void print_vector(const vector<T>& vec, const std::string& prefix) {
	cout << prefix << vec.size() << '\n';
	for (auto& it : vec) cout << prefix << '\t' << it << '\n';
}

int directions[4][2] = {
	{0, 1},
	{0, -1},
	{-1, 0},
	{1, 0}
};

vector<int> get_distances_net(const vector<char>& map, 
	char to, int height, int width)
{
	vector<int> result(map.size(), -1);
	vector<vector<int>> levels(1);
	for (size_t i = 0; i < map.size(); ++i)
		if (map[i] == to) {
			levels[0].push_back(i);
			result[i] = 0;
		}
	while (levels.back().size()) {
		levels.push_back(vector<int>());
		vector<int>& last = levels[levels.size() - 2];
		for (auto cur : last) {
			result[cur] = levels.size() - 2;
			int x = cur % width, y = cur / width;
			for (int d = 0; d != 4; ++d) {
				int dx = directions[d][0], dy = directions[d][1];
				int position = (y + dy) * width + x + dx;
				if (result[position] == -1 &&
					x + dx >= 0 && x + dx < width &&
					y + dy >= 0 && y + dy < height
				) {
					levels.back().push_back(position);
					result[position] = levels.size() - 2;
				}
			}
		}
	}
	return result;
}

void work() {
	int h, w;
	cin >> h >> w;
	vector<char> input(h * w);
	for (auto& it : input) cin >> it;
	auto dist = get_distances_net(input, '1', h, w);
	for (int y = 0; y != h; ++y) {
		for (int x = 0; x != w; ++x) {
			cout << dist[y * w + x] << ' ';
		}
		cout << '\n';
	}
}

int main() {
	cin.tie(0)->sync_with_stdio(0);
	work();
}
