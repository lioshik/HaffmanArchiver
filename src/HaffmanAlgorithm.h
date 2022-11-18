#pragma once

#include <cstdio>
#include <vector>
#include <unordered_map>

class HaffmanAlgorithm {
public:
    HaffmanAlgorithm() = delete;
    explicit HaffmanAlgorithm(const std::unordered_map<std::vector<bool>, size_t>& symbols_frequency);

    std::unordered_map<std::vector<bool>, std::vector<bool>> GetCanonicalDict() const;
    std::vector<std::vector<bool>> GetOrderedAlphabet() const;
    ~HaffmanAlgorithm();

private:
    struct Node {
        Node();
        Node(const std::vector<bool>& data, size_t weight);

        Node* Merge(const Node* other) const;

        bool IsTerminal() const;
        bool operator<(const Node& other) const;

        const Node* left_node;
        const Node* right_node;
        size_t weight;
        std::vector<bool> data;
    };

    Node* trie_root_;
    std::vector<std::pair<size_t, std::vector<bool>>> ordered_lengths_;
    std::unordered_map<std::vector<bool>, std::vector<bool>> canonical_dict_;

    void BuildCanonicalDictAndLengths();
};
