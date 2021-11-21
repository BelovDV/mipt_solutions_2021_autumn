#include <vector>
#include <cstdint>
#include <iostream>

using std::cin;
using std::cout;

// small g - what is it... I can't use variable graph anymore...
namespace graph {
// graph is set of vertexes - size_t from 0 to size
using std::vector;

using Index = size_t;
using Weight = int64_t;

const Weight kWeightMax = INT64_MAX;

struct Edge {
    Edge() = default;
    Edge(Index f, Index t) : from(f), to(t) {
    }
    Index from;
    Index to;
};
/*
Preface
there should be methods in GraphSmth, allowing:
        for (auto edge : graph.get_list_edges())
        for (auto edge : graph.get_neighbors(from))
        (bool) graph.contain(from, to)
        and several other
but checking system doesn't allow half of possibilities (I can't use begin),
so, some decisions may look weird,
but they are aligned to impossible in contest possibilities
*/

class GraphList {
public:
    explicit GraphList(size_t size) : size_(size) {
    }

public:
    size_t Size() const {
        return size_;
    }
    // doesn't consider multiedges
    void Link(Index from, Index to) {
        edges_.emplace_back(from, to);
    }

public:
    const auto& GetListEdges() const {
        return edges_;
    }
    void GetNeighbors(Index) const {
        // Preface
    }

public:
    const vector<Edge>& GetInnerInfo() const {
        return edges_;
    }

private:
    vector<Edge> edges_;
    size_t size_;
};

class GraphListWeighted : public GraphList {
public:
    explicit GraphListWeighted(size_t size) : GraphList(size) {
    }

public:
    size_t Size() const {
        return GraphList::Size();
    }
    void Link(Index from, Index to, Weight weight) {
        weights_.push_back(weight);
        GraphList::Link(from, to);
    }

public:
    // it could be get_list_edges -> iterator, which contains iterators on edges and weights and overloads ->from, ->to
    // and ->weight...
    const auto& GetListWeights() const {
        return weights_;
    }
    void GetNeighbors(Index) const {
        // Preface
    }

private:
    vector<Weight> weights_;
};

}  // namespace graph

namespace graph {
GraphListWeighted CinGraphListWeighted(bool zero_indexation) {
    size_t vertexes = 0;
    size_t edges = 0;
    std::cin >> vertexes >> edges;
    graph::GraphListWeighted graph_what(vertexes);
    while (edges--) {
        graph::Index from = 0;
        graph::Index to = 0;
        graph::Weight weight = 0;
        std::cin >> from >> to >> weight;
        if (!zero_indexation) {
            --from;
            --to;
        }
        graph_what.Link(from, to, weight);
    }
    return graph_what;
}
}  // namespace graph

namespace graph {
class FordBellman {
public:
    explicit FordBellman(const GraphListWeighted& master) : master_(master) {
    }

public:
    // without negative cycles
    vector<Index> GetDistances(Index from) const {
        vector<Index> result(master_.Size(), kWeightMax);
        result[from] = 0;
        for (size_t i = 0; i < master_.Size(); ++i) {
            auto edge = master_.GetListEdges().begin();
            auto weight = master_.GetListWeights().begin();
            for (; edge != master_.GetListEdges().end(); ++edge, ++weight) {
                if (result[edge->from] < kWeightMax) {
                    if (result[edge->to] > result[edge->from] + *weight) {
                        result[edge->to] = result[edge->from] + *weight;
                    }
                }
            }
        }
        return result;
    }

private:
    const GraphListWeighted& master_;
};
}  // namespace graph

using std::vector;

void Work() {
    auto graph_graph = graph::CinGraphListWeighted(false);
    graph::FordBellman worker(graph_graph);
    auto result = worker.GetDistances(0);
    for (auto it : result) {
        if (it == graph::kWeightMax) {
            cout << "30000\t";
        } else {
            cout << it << '\t';
        }
    }
    cout << '\n';
}

int main() {
    // cin.tie(nullptr)->sync_with_stdio(false);
    Work();
}
