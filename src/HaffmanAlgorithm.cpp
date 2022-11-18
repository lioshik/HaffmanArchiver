#include "HaffmanAlgorithm.h"

#include "BinaryHeap.h"

#include <algorithm>
#include <queue>

HaffmanAlgorithm::Node::Node() : left_node(nullptr), right_node(nullptr), weight(0), data(0) {
}

HaffmanAlgorithm::Node::Node(const std::vector<bool>& data, size_t weight)
    : left_node(nullptr), right_node(nullptr), weight(weight), data(data) {
}

HaffmanAlgorithm::Node* HaffmanAlgorithm::Node::Merge(const Node* other) const {
    auto new_node = new Node();
    new_node->weight = weight + other->weight;
    new_node->left_node = this;
    new_node->right_node = other;
    if (new_node->left_node->data < new_node->right_node->data) {
        new_node->data = new_node->left_node->data;
    } else {
        new_node->data = new_node->right_node->data;
    }
    return new_node;
}

bool HaffmanAlgorithm::Node::operator<(const Node& other) const {
    if (weight == other.weight) {
        return data < other.data;
    }
    return weight < other.weight;
}

bool HaffmanAlgorithm::Node::IsTerminal() const {
    return left_node == nullptr && right_node == nullptr;
}

HaffmanAlgorithm::HaffmanAlgorithm(const std::unordered_map<std::vector<bool>, size_t>& symbols_frequency) {
    struct NodeComparator {
        bool operator()(const Node* node1, const Node* node2) const {
            return (*node1) < (*node2);
        }
    };
    BinaryHeap<Node*, NodeComparator> queue;

    for (const auto& [symbol, weight] : symbols_frequency) {
        queue.Push(new Node(symbol, weight));
    }
    while (queue.Size() > 1) {
        Node* node1 = queue.GetFront();
        queue.Pop();
        Node* node2 = queue.GetFront();
        queue.Pop();
        queue.Push(node1->Merge(node2));
    }
    trie_root_ = queue.GetFront();

    BuildCanonicalDictAndLengths();
}

std::unordered_map<std::vector<bool>, std::vector<bool>> HaffmanAlgorithm::GetCanonicalDict() const {
    return canonical_dict_;
}

HaffmanAlgorithm::~HaffmanAlgorithm() {
    if (trie_root_ == nullptr) {
        return;
    }
    std::queue<const Node*> clear_queue;
    clear_queue.push(trie_root_);

    while (!clear_queue.empty()) {
        auto current_node = clear_queue.front();
        clear_queue.pop();
        if (current_node->left_node != nullptr) {
            clear_queue.push(current_node->left_node);
        }
        if (current_node->right_node != nullptr) {
            clear_queue.push(current_node->right_node);
        }
        delete current_node;
    }
}

std::vector<std::vector<bool>> HaffmanAlgorithm::GetOrderedAlphabet() const {
    std::vector<std::vector<bool>> result;
    result.reserve(ordered_lengths_.size());
    for (const auto& length_data : ordered_lengths_) {
        result.push_back(length_data.second);
    }
    return result;
}

void HaffmanAlgorithm::BuildCanonicalDictAndLengths() {
    struct NodeLength {
        const Node* node;
        size_t length;
    };

    std::queue<NodeLength> queue;
    queue.push({.node = trie_root_, .length = 0});
    while (!queue.empty()) {
        const Node* current_node = queue.front().node;
        size_t current_length = queue.front().length;
        queue.pop();

        if (current_node->IsTerminal()) {
            ordered_lengths_.push_back({
                current_length,
                current_node->data,
            });
        } else {
            queue.push({.node = current_node->left_node, .length = current_length + 1});
            queue.push({.node = current_node->right_node, .length = current_length + 1});
        }
    }

    std::sort(ordered_lengths_.begin(), ordered_lengths_.end());

    std::vector<bool> current_code;
    for (const auto& [length, data] : ordered_lengths_) {
        size_t i = current_code.size();
        while (i > 0 && current_code[i - 1]) {
            current_code[i - 1] = false;
            --i;
        }
        if (i > 0) {
            current_code[i - 1] = true;
        }
        while (current_code.size() < length) {
            current_code.push_back(false);
        }

        canonical_dict_[data] = current_code;
    }
}
