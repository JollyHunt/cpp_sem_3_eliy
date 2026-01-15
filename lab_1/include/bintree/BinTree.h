#ifndef BINTREE_H
#define BINTREE_H

#include <memory>
#include <stdexcept>

template<typename T>
class BinTree {
private:
    struct Node {
        T value;
        size_t count = 1;
        size_t left_size = 0;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        explicit Node(T v) : value(std::move(v)) {}
    };

    std::unique_ptr<Node> root_;
    size_t total_size_ = 0;

    void insert_impl(std::unique_ptr<Node>& node, T value) {
        if (!node) {
            node = std::make_unique<Node>(std::move(value));
            total_size_++;
            return;
        }

        if (value == node->value) {
            node->count++;
            total_size_++;
        } else if (value < node->value) {
            node->left_size++;
            insert_impl(node->left, std::move(value));
        } else {
            insert_impl(node->right, std::move(value));
        }
    }

    const T& kth_element_impl(const Node* node, size_t k) const {
        size_t left_count = node->left_size;

        if (k < left_count) {
            return kth_element_impl(node->left.get(), k);
        } else if (k < left_count + node->count) {
            return node->value;
        } else {
            return kth_element_impl(node->right.get(), k - left_count - node->count);
        }
    }

public:
    void insert(T value) {
        insert_impl(root_, std::move(value));
    }

    size_t size() const {
        return total_size_;
    }

    bool empty() const {
        return total_size_ == 0;
    }

    T median() const {
        if (empty()) throw std::runtime_error("Median of empty tree");
        size_t n = total_size_;
        if (n % 2 == 1) {
            return kth_element_impl(root_.get(), n / 2);
        } else {
            T a = kth_element_impl(root_.get(), n / 2 - 1);
            T b = kth_element_impl(root_.get(), n / 2);
            return (a + b) / T{2};
        }
    }
};

#endif