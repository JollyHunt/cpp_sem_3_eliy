#ifndef READ_ONLY_STREAM_H
#define READ_ONLY_STREAM_H

#include <string>
#include "Stream.h"
#include "../sequence/Sequence.h"
#include "../sequence/LazySequence.h"
#include "Deserializer.h"
#include <sstream>
#include <vector>

template <typename T>
class ReadOnlyStream {
public:
    enum class SourceType {
        SEQUENCE,
        LAZY_SEQUENCE,
        STRING
    };

private:
    SourceType source_type_;
    Sequence<T>* seq_ = nullptr;
    LazySequence<T>* lazy_seq_ = nullptr;
    std::string string_data_;
    std::istringstream* string_stream_ = nullptr;
    size_t position_ = 0;
    bool is_open_ = false;
    DataType data_type_;

    ReadOnlyStream(const ReadOnlyStream&) = delete;
    ReadOnlyStream& operator=(const ReadOnlyStream&) = delete;

public:
    explicit ReadOnlyStream(Sequence<T>* seq, DataType type = DataType::INT)
        : source_type_(SourceType::SEQUENCE), seq_(seq), is_open_(true), data_type_(type) {}

    explicit ReadOnlyStream(LazySequence<T>* lazy_seq, DataType type = DataType::INT)
        : source_type_(SourceType::LAZY_SEQUENCE), lazy_seq_(lazy_seq), is_open_(true), data_type_(type) {}

    explicit ReadOnlyStream(const std::string& data, DataType type = DataType::INT)
        : source_type_(SourceType::STRING), string_data_(data), is_open_(true), data_type_(type) {
        string_stream_ = new std::istringstream(data);
    }

    ~ReadOnlyStream() {
        delete string_stream_;
    }

    void Open() { is_open_ = true; }

    void Close() { is_open_ = false; }

    bool IsEndOfStream() const {
        if (!is_open_) return true;
        switch (source_type_) {
            case SourceType::SEQUENCE:
                return position_ >= seq_->Size();
            case SourceType::LAZY_SEQUENCE: {
                size_t len = lazy_seq_->GetMaterializedCount();
                return position_ >= len;
            }
            case SourceType::STRING:
                return string_stream_->eof();
        }
        return true;
    }

    T Read() {
        if (!is_open_ || IsEndOfStream()) {
            throw EndOfStreamException();
        }

        T value{};
        std::string token;

        switch (source_type_) {
            case SourceType::SEQUENCE:
                value = seq_->Get(position_);
                break;
            case SourceType::LAZY_SEQUENCE:
                value = lazy_seq_->Get(position_);
                break;
            case SourceType::STRING:
                *string_stream_ >> token;
                value = Deserializer<T>::deserialize(token, data_type_);
                break;
        }
        ++position_;
        return value;
    }

    size_t GetPosition() const { return position_; }

    bool IsCanSeek() const {
        return source_type_ != SourceType::STRING;
    }

    size_t Seek(size_t index) {
        if (!IsCanSeek()) {
            throw std::runtime_error("Seek not supported for string source");
        }
        switch (source_type_) {
            case SourceType::SEQUENCE:
                position_ = (index > seq_->Size()) ? seq_->Size() : index;
                break;
            case SourceType::LAZY_SEQUENCE: {
                size_t max_pos = lazy_seq_->GetMaterializedCount();
                position_ = max_pos;
                break;
            }
            default:
                break;
        }
        return position_;
    }

    bool IsCanGoBack() const {
        return source_type_ != SourceType::STRING;
    }
};

#endif