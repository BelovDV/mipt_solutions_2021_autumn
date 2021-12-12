#include <vector>
#include <iostream>
#include <memory>
#include <array>

// ===== // ===== // ===== // ===== // ===== // ===== // ===== // ===== //

#ifndef MY
#define LOG(var)
#define LOG_IN
#define LOG_OUT
#else
#define DEBUG
int log_depth = 0;
#define LOG_IN                                \
    for (int i = 0; i < log_depth; ++i) {     \
        std::cout << "  ";                    \
    }                                         \
    std::cout << __PRETTY_FUNCTION__ << '\n'; \
    ++log_depth;
#define LOG_OUT --log_depth;
#define LOG(var)                          \
    for (int i = 0; i < log_depth; ++i) { \
        std::cout << "  ";                \
    }                                     \
    std::cout << "LOG: " << __LINE__ << " \t" << #var << ": \t" << var << "\n";
#endif

template <typename Cont, typename = typename std::enable_if_t<!std::is_same<Cont, std::string>::value>>
auto operator<<(std::ostream& out, const Cont& cont) -> decltype(cont.begin(), out) {
    for (auto it : cont) {
        out << it << '\t';
    }
    out << '\n';
    return out;
}
template <typename Cont>
auto operator<<(std::ostream& out, const Cont& cont) -> decltype(cont.first, cont.second, out) {
    out << cont.first << ' ' << cont.second;
    return out;
}

// ===== // ===== // ===== // ===== // ===== // ===== // ===== // ===== //

class ITrieNode {
public:
    using Symbol = char;
    using String = std::string;

public:
    virtual ~ITrieNode() = default;

public:
    virtual ITrieNode* Next(Symbol symbol) = 0;
    virtual ITrieNode* Create(Symbol symbol, ITrieNode* suffix) = 0;
    virtual ITrieNode* Suffix() = 0;
    virtual size_t Depth() = 0;
};

// ===== // ===== // ===== // ===== // ===== // ===== // ===== // ===== //

class Trie {
public:
    using Symbol = ITrieNode::Symbol;
    using String = ITrieNode::String;

public:
    explicit Trie(std::unique_ptr<ITrieNode>&& root) : root_(std::move(root)) {
    }

public:
    bool Contain(const String& pattern) const {
        ITrieNode* iter = root_.get();
        for (auto symbol : pattern) {
            if (!(iter->Next(symbol))) {
                return false;
            }
            iter = iter->Next(symbol);
        }
        return true;
    }

    bool Insert(const String& inserted) {
        LOG_IN
        ITrieNode* iter = root_.get();
        bool was_inserted = false;
        for (auto symbol : inserted) {
            LOG(symbol)
            if (!(iter->Next(symbol))) {
                was_inserted = true;
                iter->Create(symbol, Next(iter, symbol));
            }
            iter = iter->Next(symbol);
        }
        LOG_OUT
        return was_inserted;
    }

    auto PrefixFunction(const String& pattern) {
        LOG_IN
        Insert(pattern);
        LOG("start")
        std::vector<size_t> result;
        ITrieNode* iter = root_.get();
        for (auto symbol : pattern) {
            iter = iter->Next(symbol);
            result.push_back(iter->Suffix()->Depth());
        }
        LOG_OUT
        return result;
    }

private:
    ITrieNode* Next(ITrieNode* from, Symbol to) {
        if (from->Next(to)) {
            return from->Next(to);
        }
        if (from == root_.get()) {
            return from;
        }
        return Next(from->Suffix(), to);
    }

private:
    std::unique_ptr<ITrieNode> root_;
};

// ===== // ===== // ===== // ===== // ===== // ===== // ===== // ===== //

class Node : public ITrieNode {
public:
    explicit Node(size_t depth = 0) : depth_(depth) {
    }

    ~Node() override = default;

public:
    ITrieNode* Next(Symbol symbol) override {
        return At(symbol).get();
    }
    ITrieNode* Create(Symbol symbol, ITrieNode* suffix) override {
        At(symbol) = std::make_unique<Node>();
        At(symbol)->depth_ = depth_ + 1;
        At(symbol)->suffix_ = suffix;
        return At(symbol).get();
    }
    ITrieNode* Suffix() override {
        return suffix_;
    }
    size_t Depth() override {
        return depth_;
    }

private:
    std::unique_ptr<Node>& At(Symbol symbol) {
#ifdef DEBUG
        return next_.at(symbol - 'a');
#else
        return next_[symbol - 'a'];
#endif
    }

private:
    std::array<std::unique_ptr<Node>, 26> next_;
    ITrieNode* suffix_;
    size_t depth_;
};

// ===== // ===== // ===== // ===== // ===== // ===== // ===== // ===== //

class NodeLine : public ITrieNode {
public:
    explicit NodeLine(size_t depth = 0) : depth_(depth) {
    }
    ~NodeLine() override = default;

public:
    ITrieNode* Next(Symbol symbol) override {
        if (symbol == direction_) {
            return next_.get();
        }
        return nullptr;
    }
    ITrieNode* Create(Symbol symbol, ITrieNode* suffix) override {
        direction_ = symbol;
        next_ = std::make_unique<NodeLine>();
        next_->depth_ = depth_ + 1;
        next_->suffix_ = suffix;
        return next_.get();
    }
    ITrieNode* Suffix() override {
        return suffix_;
    }
    size_t Depth() override {
        return depth_;
    }

private:
    Symbol direction_;
    std::unique_ptr<NodeLine> next_;
    ITrieNode* suffix_;
    size_t depth_;
};

// ===== // ===== // ===== // ===== // ===== // ===== // ===== // ===== //

using std::cin;
using std::cout;
using std::vector;

void WorkTrie() {
}

int main() {
#ifndef MY
    cin.tie(nullptr)->sync_with_stdio(false);
#endif
    WorkTrie();
}