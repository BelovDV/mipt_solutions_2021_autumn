#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

class DSU {
public:
    explicit DSU(size_t count) : parent_(std::vector<size_t>(count)), size_(std::vector<size_t>(count)) {
        for (size_t i = 0; i < count; ++i) {
            parent_[i] = i, size_[i] = 0;
        }
    }

public:
    size_t Root(size_t element) {
        if (parent_[element] != element) {
            parent_[element] = Root(parent_[element]);
        }
        return parent_[element];
    }
    void Unite(size_t el_1, size_t el_2) {
        size_t r1 = Root(el_1);
        size_t r2 = Root(el_2);
        if (size_[r1] > size_[r2]) {
            std::swap(r1, r2);
        }
        size_[r1] += size_[r2] + 1;
        parent_[r2] = r1;
    }

private:
    std::vector<size_t> parent_;
    std::vector<size_t> size_;
};

struct Edge {
    int from, to;
};

void Solution() {
}

void Work() {
    int n = 0;
    int m = 0;
    cin >> n >> m;
    DSU dsu(n);
    vector<int> sum(n, 0);
    for (int i = 0; i < m; ++i) {
        int code = 0;
        cin >> code;
        if (code == 1) {
            int x = 0;
            int y = 0;
            int w = 0;
            cin >> x >> y >> w;
            int r_x = dsu.Root(x - 1);
            int r_y = dsu.Root(y - 1);
            if (r_x == r_y) {
                sum[r_x] += w;
            } else {
                int vsp = sum[r_x] + sum[r_y];
                sum[r_x] = sum[r_y] = vsp;
                dsu.Unite(r_x, r_y);
                sum[dsu.Root(r_x)] += w;
            }
        } else {
            int x = 0;
            cin >> x;
            cout << sum[dsu.Root(x - 1)] << '\n';
        }
    }
}

int main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    Work();
}
