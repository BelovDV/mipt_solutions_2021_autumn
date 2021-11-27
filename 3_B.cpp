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

void Work() {
    int vertex_count = 0;
    int edges_count = 0;
    cin >> vertex_count >> edges_count;
    DSU dsu(vertex_count);
    vector<int> component_sum(vertex_count, 0);
    for (int i = 0; i < edges_count; ++i) {
        int operation = 0;
        cin >> operation;
        if (operation == 1) {
            int vert_1 = 0;
            int vert_2 = 0;
            int weight = 0;
            cin >> vert_1 >> vert_2 >> weight;
            int root_1 = dsu.Root(vert_1 - 1);
            int root_2 = dsu.Root(vert_2 - 1);
            if (root_1 == root_2) {
                component_sum[root_1] += weight;
            } else {
                int vsp = component_sum[root_1] + component_sum[root_2];
                component_sum[root_1] = component_sum[root_2] = vsp;
                dsu.Unite(root_1, root_2);
                component_sum[dsu.Root(root_1)] += weight;
            }
        } else {
            int vertex = 0;
            cin >> vertex;
            cout << component_sum[dsu.Root(vertex - 1)] << '\n';
        }
    }
}

int main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    Work();
}
