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

int Solution(int n, const vector<Edge>& edges) {
    int remain = n - 1;
    DSU dsu(n);
    int iter = 0;
    while (remain) {
        int root_from = dsu.Root(edges[iter].from);
        int root_to = dsu.Root(edges[iter].to);

        if (root_from != root_to) {
            --remain;
            dsu.Unite(root_from, root_to);
        }

        ++iter;
    }
    return iter;
}

void Work() {
    int n = 0;
    int m = 0;
    cin >> n >> m;
    vector<Edge> edges(m);
    for (auto& it : edges) {
        cin >> it.from >> it.to;
    }
    cout << Solution(n, edges);
}

int main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    Work();
}
