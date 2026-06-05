// Copyright 2022 NNTU-CS
#include  <iostream>
#include  <fstream>
#include  <locale>
#include  <cstdlib>
#include  "tree.h"
#include <algorithm>
#include <stdexcept>

static long long factorial(int n) {
    long long result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

void PMTree::build(Node* node, const std::vector<char>& remaining) {
    if (remaining.empty()) {
        return;
    }
    for (size_t i = 0; i < remaining.size(); ++i) {
        Node* child = new Node(remaining[i]);
        node->children.push_back(child);
        std::vector<char> next_remaining;
        next_remaining.reserve(remaining.size() - 1);
        for (size_t j = 0; j < remaining.size(); ++j) {
            if (j != i) {
                next_remaining.push_back(remaining[j]);
            }
        }
        build(child, next_remaining);
    }
}

PMTree::PMTree() : root_(nullptr), size_(0) {}

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

void PMTree::destroy(Node* node) {
    if (node != nullptr) {
        for (Node* child : node->children) {
            destroy(child);
        }
        delete node;
    }
}

static void collectAllPerms(const Node* node, std::vector<char>& current,
    std::vector<std::vector<char>>& result) {
    if (node->children.empty()) {
        result.push_back(current);
        return;
    }
    for (const Node* child : node->children) {
        current.push_back(child->value);
        collectAllPerms(child, current, result);
        current.pop_back();
    }
}

std::vector<std::vector<char>> getAllPerms(const PMTree& tree) {
    std::vector<std::vector<char>> result;
    if (tree.root() == nullptr) {
        return result;
    }
    std::vector<char> path;
    collectAllPerms(tree.root(), path, result);
    return result;
}

static bool findPermByCount(const Node* node, int& count, int target,
    std::vector<char>& current,
    std::vector<char>& out) {
    if (node->children.empty()) {
        ++count;
        if (count == target) {
            out = current;
            return true;
        }
        return false;
    }
    for (const Node* child : node->children) {
        current.push_back(child->value);
        if (findPermByCount(child, count, target, current, out)) {
            return true;
        }
        current.pop_back();
    }
    return false;
}

std::vector<char> getPerm1(const PMTree& tree, int num) {
    if (tree.root() == nullptr || num <= 0) {
        return {};
    }
    std::vector<char> current;
    std::vector<char> result;
    int count = 0;
    findPermByCount(tree.root(), count, num, current, result);
    return result;
}

std::vector<char> getPerm2(const PMTree& tree, int num) {
    if (tree.root() == nullptr || num <= 0) {
        return {};
    }
    int n = tree.size();
    long long total = factorial(n);
    if (num > total) {
        return {};
    }

    std::vector<char> result;
    const Node* current = tree.root();
    int remaining = n;
    long long target = num;

    while (remaining > 0) {
        long long block_size = factorial(remaining - 1);
        int child_index = 0;
        while (target > block_size) {
            target -= block_size;
            ++child_index;
        }
        const Node* next_node = current->children[child_index];
        result.push_back(next_node->value);
        current = next_node;
        --remaining;
    }
    return result;
}
