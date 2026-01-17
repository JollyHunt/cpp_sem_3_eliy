#ifndef CUSTOM_HASH_TABLE_H
#define CUSTOM_HASH_TABLE_H

#include "sequence/ArraySequence.h"
#include "sequence/ListSequence.h"

template <typename T>
size_t simpleHash(const ArraySequence<T>& key) {
    size_t hash = 0;
    for (size_t i = 0; i < key.Size(); ++i) {
        hash = hash * 31 + static_cast<size_t>(key.Get(i));
    }
    return hash;
}

template <typename TKey, typename TValue>
class CustomHashTable {
private:
    struct Entry {
        TKey key;
        TValue value;
        Entry(const TKey& k, const TValue& v) : key(k), value(v) {}
    };

    ArraySequence<ListSequence<Entry>> buckets_;
    size_t size_;
    size_t capacity_;

    size_t hash(const TKey& key) const {
        size_t h = simpleHash(key);
        return h % capacity_;
    }

    void resize() {
        size_t oldCap = capacity_;
        capacity_ *= 2;
        ArraySequence<ListSequence<Entry>> newBuckets(capacity_);

        for (size_t i = 0; i < oldCap; ++i) {
            ListSequence<Entry> bucket = buckets_.Get(i);
            for (size_t j = 0; j < bucket.Size(); ++j) {
                const Entry& e = bucket.Get(j);
                size_t idx = hash(e.key);
                newBuckets.Get(idx).Append(e);
            }
        }
        buckets_ = newBuckets;
    }

public:
    CustomHashTable(size_t initial_capacity = 16)
        : buckets_(initial_capacity), size_(0), capacity_(initial_capacity) {
        for (size_t i = 0; i < capacity_; ++i) {
            buckets_.Set(i, ListSequence<Entry>());
        }
    }

    void Add(const TKey& key, const TValue& value) {
        if (size_ >= capacity_) resize();
        size_t idx = hash(key);
        ListSequence<Entry> bucket = buckets_.Get(idx);

        bool found = false;
        for (size_t i = 0; i < bucket.Size(); ++i) {
            if (bucket.Get(i).key == key) {
                bucket.RemoveAt(i);
                bucket.InsertAt(i, Entry(key, value));
                found = true;
                break;
            }
        }

        if (!found) {
            bucket.Append(Entry(key, value));
        }

        buckets_.Set(idx, bucket);
        if (!found) ++size_;
    }

    TValue Get(const TKey& key) const {
        size_t idx = hash(key);
        const ListSequence<Entry>& bucket = buckets_.Get(idx);
        for (size_t i = 0; i < bucket.Size(); ++i) {
            if (bucket.Get(i).key == key) {
                return bucket.Get(i).value;
            }
        }
        throw "Key not found";
    }

    bool ContainsKey(const TKey& key) const {
        size_t idx = hash(key);
        const ListSequence<Entry>& bucket = buckets_.Get(idx);
        for (size_t i = 0; i < bucket.Size(); ++i) {
            if (bucket.Get(i).key == key) return true;
        }
        return false;
    }

    void Remove(const TKey& key) {
        size_t idx = hash(key);
        ListSequence<Entry> bucket = buckets_.Get(idx);
        bool found = false;
        for (size_t i = 0; i < bucket.Size(); ++i) {
            if (bucket.Get(i).key == key) {
                bucket.RemoveAt(i);
                found = true;
                break;
            }
        }
        if (!found) throw "Key not found";
        buckets_.Set(idx, bucket);
        --size_;
    }

    size_t GetCount() const { return size_; }
    size_t GetCapacity() const { return capacity_; }

    ArraySequence<TKey> GetKeys() const {
        ArraySequence<TKey> keys;
        for (size_t i = 0; i < capacity_; ++i) {
            const ListSequence<Entry>& bucket = buckets_.Get(i);
            for (size_t j = 0; j < bucket.Size(); ++j) {
                keys.Append(bucket.Get(j).key);
            }
        }
        return keys;
    }

    ArraySequence<TValue> GetValues() const {
        ArraySequence<TValue> values;
        for (size_t i = 0; i < capacity_; ++i) {
            const ListSequence<Entry>& bucket = buckets_.Get(i);
            for (size_t j = 0; j < bucket.Size(); ++j) {
                values.Append(bucket.Get(j).value);
            }
        }
        return values;
    }
};

#endif