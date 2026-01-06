#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <cstddef>
#include <stdexcept>

template <typename T>
class Sequence {
public:
    using size_type = size_t;

    virtual ~Sequence() = default;

    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    virtual T Get(size_type index) const = 0;
    virtual void Set(size_type index, const T& value) = 0;

    virtual void Append(const T& item) = 0;
    virtual void Prepend(const T& item) = 0;
    virtual void InsertAt(size_type index, const T& item) = 0;
    virtual void RemoveAt(size_type index) = 0;

    virtual size_type Size() const = 0;
    virtual bool IsEmpty() const = 0;
    virtual void Clear() = 0;
};

#endif