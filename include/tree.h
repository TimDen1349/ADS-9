#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_

#include <vector>

class PMTree {
public:
    struct Node {
        char data;
        std::vector<Node*> children;
        explicit Node(char c) : data(c) {}
    };
    explicit PMTree(const std::vector<char>& elements);
    ~PMTree();
    PMTree(const PMTree&) = delete;
    PMTree& operator=(const PMTree&) = delete;
    const Node* root() const { return root_; }
    int size() const { return size_; }

private:
    Node* root_;
    int size_;
    void build(Node* node, const std::vector<char>& remaining);
    void destroy(Node* node);
};

std::vector<std::vector<char>> getAllPerms(const PMTree& tree);
std::vector<char> getPerm1(const PMTree& tree, int num);
std::vector<char> getPerm2(const PMTree& tree, int num);

#endif
