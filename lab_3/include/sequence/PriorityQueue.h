#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "Sequence.h"
#include <functional>
#include <stdexcept>

template <typename T, typename K>
class PriorityQueue : public Sequence<T> {
private:
    struct Node {
        T data;
        K priority;
        Node* next;
        Node(const T& d, const K& p) : data(d), priority(p), next(nullptr) {}
    };

    Node* head_;
    size_t size_;
    std::function<bool(const K&, const K&)> comp_;

    T Get(size_t index) const override {
        throw std::logic_error("Get is not allowed in PriorityQueue");
    }

    T GetFirst() const override {
        if (IsEmpty()) throw std::out_of_range("PriorityQueue is empty");
        return head_->data;
    }

    T GetLast() const override {
        throw std::logic_error("GetLast is not allowed in PriorityQueue");
    }

    void Set(size_t index, const T& value) override {
        throw std::logic_error("Set is not allowed in PriorityQueue");
    }

    void Append(const T& item) override {
        throw std::logic_error("Append is not allowed in PriorityQueue");
    }

    void Prepend(const T& item) override {
        throw std::logic_error("Prepend is not allowed in PriorityQueue");
    }

    void InsertAt(size_t index, const T& item) override {
        throw std::logic_error("InsertAt is not allowed in PriorityQueue");
    }

    void RemoveAt(size_t index) override {
        throw std::logic_error("RemoveAt is not allowed in PriorityQueue");
    }

public:
    using size_type = size_t;

    explicit PriorityQueue(std::function<bool(const K&, const K&)> comp = std::less<K>())
        : head_(nullptr), size_(0), comp_(comp) {}

    ~PriorityQueue() {
        Clear();
    }

    T Peek(size_t i) const {
        if (i >= size_) throw std::out_of_range("Index out of range");
        Node* curr = head_;
        for (size_t idx = 0; idx < i; ++idx) {
            curr = curr->next;
        }
        return curr->data;
    }

    T PeekFirst() const {
        return GetFirst();
    }

    T PeekLast() const {
        if (IsEmpty()) throw std::out_of_range("PriorityQueue is empty");
        Node* curr = head_;
        while (curr->next) {
            curr = curr->next;
        }
        return curr->data;
    }

    void Enqueue(const T& item, const K& priority) {
        Node* newNode = new Node(item, priority);
        if (!head_ || comp_(priority, head_->priority)) {
            newNode->next = head_;
            head_ = newNode;
        } else {
            Node* curr = head_;
            while (curr->next && !comp_(priority, curr->next->priority)) {
                curr = curr->next;
            }
            newNode->next = curr->next;
            curr->next = newNode;
        }
        ++size_;
    }

    T Dequeue() {
        if (IsEmpty()) throw std::out_of_range("PriorityQueue is empty");
        Node* temp = head_;
        T result = temp->data;
        head_ = head_->next;
        delete temp;
        --size_;
        return result;
    }

    size_type Size() const override {
        return size_;
    }

    bool IsEmpty() const override {
        return size_ == 0;
    }

    void Clear() override {
        while (head_) {
            Node* temp = head_;
            head_ = head_->next;
            delete temp;
        }
        size_ = 0;
    }
};

#endif