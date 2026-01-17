#ifndef LIST_SEQUENCE_H
#define LIST_SEQUENCE_H

#include "Sequence.h"

template <typename T>
class ListSequence : public Sequence<T> {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* head_;
    size_t size_;

public:
    using size_type = size_t;

    ListSequence() : head_(nullptr), size_(0) {}

    ListSequence(const T* items, size_type count) : ListSequence() {
        for (size_type i = 0; i < count; ++i) {
            Append(items[i]);
        }
    }

    explicit ListSequence(const Sequence<T>* other) : ListSequence() {
        if (!other) return;
        for (size_t i = 0; i < other->Size(); ++i) {
            Append(other->Get(i));
        }
    }

    ListSequence(const ListSequence& other) : ListSequence() {
        Node* curr = other.head_;
        while (curr) {
            Append(curr->data);
            curr = curr->next;
        }
    }

    ListSequence& operator=(const ListSequence& other) {
        if (this == &other) return *this;
        Clear();
        Node* curr = other.head_;
        while (curr) {
            Append(curr->data);
            curr = curr->next;
        }
        return *this;
    }

    ~ListSequence() {
        Clear();
    }

    T GetFirst() const override {
        if (IsEmpty()) throw std::out_of_range("Sequence is empty");
        return head_->data;
    }

    T GetLast() const override {
        if (IsEmpty()) throw std::out_of_range("Sequence is empty");
        Node* curr = head_;
        while (curr->next) curr = curr->next;
        return curr->data;
    }

    T Get(size_type index) const override {
        if (index >= size_) throw std::out_of_range("Index out of range");
        Node* curr = head_;
        for (size_type i = 0; i < index; ++i) curr = curr->next;
        return curr->data;
    }

    void Set(size_type index, const T& value) override {
        if (index >= size_) throw std::out_of_range("Index out of range");
        Node* curr = head_;
        for (size_type i = 0; i < index; ++i) curr = curr->next;
        curr->data = value;
    }

    void Append(const T& item) override {
        Node* newNode = new Node(item);
        if (!head_) head_ = newNode;
        else {
            Node* curr = head_;
            while (curr->next) curr = curr->next;
            curr->next = newNode;
        }
        ++size_;
    }

    void Prepend(const T& item) override {
        Node* newNode = new Node(item);
        newNode->next = head_;
        head_ = newNode;
        ++size_;
    }

    void InsertAt(size_type index, const T& item) override {
        if (index > size_) throw std::out_of_range("Insert index out of range");
        if (index == 0) {
            Prepend(item);
            return;
        }
        Node* curr = head_;
        for (size_type i = 0; i < index - 1; ++i) curr = curr->next;
        Node* newNode = new Node(item);
        newNode->next = curr->next;
        curr->next = newNode;
        ++size_;
    }

    void RemoveAt(size_type index) override {
        if (index >= size_) throw std::out_of_range("Remove index out of range");
        if (index == 0) {
            Node* temp = head_;
            head_ = head_->next;
            delete temp;
        } else {
            Node* curr = head_;
            for (size_type i = 0; i < index - 1; ++i) curr = curr->next;
            Node* toDelete = curr->next;
            curr->next = toDelete->next;
            delete toDelete;
        }
        --size_;
    }

    size_type Size() const override { return size_; }

    bool IsEmpty() const override { return size_ == 0; }

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