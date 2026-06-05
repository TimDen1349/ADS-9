// Copyright 2026 NNTU-CS
#include <cstdint>
#include <algorithm>
#include <vector>
#include "tree.h"

namespace {

int64_t factorial(int n) {
    int64_t result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

    void collectAll(const PMTree::Node* node, std::vector<char>& current,
        std::vector<std::vector<char>>& out) {
        if (node->children.empty()) {
            out.push_back(current);
            return;
        }
        for (const auto* child : node->children) {
            current.push_back(child->data);
            collectAll(child, current, out);
            current.pop_back();
        }
    }

    bool findNth(const PMTree::Node* node, int& counter, int target,
        std::vector<char>& path, std::vector<char>& result) {
        if (node->children.empty()) {
            ++counter;
            if (counter == target) {
                result = path;
                return true;
            }
            return false;
        }
        for (const auto* child : node->children) {
            path.push_back(child->data);
            if (findNth(child, counter, target, path, result)) {
                return true;
            }
            path.pop_back();
        }
        return false;
    }

}  // namespace

PMTree::PMTree(const std::vector<char>& elements) : root_(nullptr), size_(0) {
    if (elements.empty()) {
        return;
    }

    std::vector<char> sorted = elements;
    std::sort(sorted.begin(), sorted.end());
    size_ = static_cast<int>(sorted.size());

    root_ = new Node('\0');
    build(root_, sorted);
}

PMTree::~PMTree() {
    destroy(root_);
}

void PMTree::build(Node* node, const std::vector<char>& remaining) {
    if (remaining.empty()) return;
    for (size_t i = 0; i < remaining.size(); ++i) {
        auto* child = new Node(remaining[i]);
        node->children.push_back(child);

        std::vector<char> next_remaining = remaining;
        next_remaining.erase(next_remaining.begin() + i);
        build(child, next_remaining);
    }
}

void PMTree::destroy(Node* node) {
    if (node == nullptr) return;
    for (auto* child : node->children) {
        destroy(child);
    }
    delete node;
}

std::vector<std::vector<char>> getAllPerms(const PMTree& tree) {
    std::vector<std::vector<char>> result;
    if (tree.root() == nullptr) {
        return result;
    }
    std::vector<char> path;
    collectAll(tree.root(), path, result);
    return result;
}

std::vector<char> getPerm1(const PMTree& tree, int num) {
    const auto* root = tree.root();
    if (root == nullptr || num <= 0 || num > factorial(tree.size())) {
        return {};
    }
    std::vector<char> path, result;
    int counter = 0;
    findNth(root, counter, num, path, result);
    return result;
}

std::vector<char> getPerm2(const PMTree& tree, int num) {
    const auto* root = tree.root();
    int n = tree.size();
    if (root == nullptr || num <= 0 || num > factorial(n)) {
        return {};
    }
    std::vector<char> result;
    result.reserve(n);
    const PMTree::Node* current = root;
    int64_t k = num - 1;

    for (int i = 1; i <= n; ++i) {
        int64_t block = factorial(n - i);
        int idx = static_cast<int>(k / block);
        k %= block;
        current = current->children[idx];
        result.push_back(current->data);
    }
    return result;
}
