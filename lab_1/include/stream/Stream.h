#pragma once
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include "sequence/LazySequence.h"
#include "sequence/Generator.h"

template<typename T>
class Stream {
public:
    static std::unique_ptr<LazySequence<T>> from_cin() {
        std::vector<T> items;
        T item;
        while (std::cin >> item) {
            items.push_back(item);
            if (std::cin.peek() == '\n') break;
        }
        return std::make_unique<LazySequence<T>>(items.data(), items.size());
    }

    static std::unique_ptr<LazySequence<T>> from_file(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file");
        }
        std::vector<T> items;
        T item;
        while (file >> item) {
            items.push_back(item);
        }
        return std::make_unique<LazySequence<T>>(items.data(), items.size());
    }

    static std::unique_ptr<LazySequence<T>> from_binary(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open binary file");
        }
        std::vector<T> items;
        T item;
        while (file.read(reinterpret_cast<char*>(&item), sizeof(T))) {
            items.push_back(item);
        }
        return std::make_unique<LazySequence<T>>(items.data(), items.size());
    }

    static void print_to_cout(const std::unique_ptr<LazySequence<T>>& seq) {
        size_t count = seq->GetMaterializedCount();
        for (size_t i = 0; i < count; ++i) {
            std::cout << seq->Get(i) << " ";
        }
        std::cout << "\n";
    }

    static void print_to_file(const std::unique_ptr<LazySequence<T>>& seq, const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot create file");
        }
        size_t count = seq->GetMaterializedCount();
        for (size_t i = 0; i < count; ++i) {
            file << seq->Get(i) << " ";
        }
        file << "\n";
    }

    static void print_to_binary(const std::unique_ptr<LazySequence<T>>& seq, const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot create binary file");
        }
        size_t count = seq->GetMaterializedCount();
        for (size_t i = 0; i < count; ++i) {
            T val = seq->Get(i);
            file.write(reinterpret_cast<const char*>(&val), sizeof(T));
        }
    }
};