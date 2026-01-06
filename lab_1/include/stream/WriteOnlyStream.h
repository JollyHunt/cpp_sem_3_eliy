#ifndef WRITE_ONLY_STREAM_H
#define WRITE_ONLY_STREAM_H

#include <string>
#include "Stream.h"
#include "../sequence/Sequence.h"
#include "Serializer.h"

template <typename T>
class WriteOnlyStream {
public:
    enum class SinkType {
        SEQUENCE,
        STRING
    };

private:
    SinkType sink_type_;
    Sequence<T>* seq_ = nullptr;
    std::string* output_string_ = nullptr;
    size_t position_ = 0;
    bool is_open_ = false;
    DataType data_type_;

    WriteOnlyStream(const WriteOnlyStream&) = delete;
    WriteOnlyStream& operator=(const WriteOnlyStream&) = delete;

public:
    explicit WriteOnlyStream(Sequence<T>* seq, DataType type = DataType::INT)
        : sink_type_(SinkType::SEQUENCE), seq_(seq), is_open_(true), data_type_(type) {}

    explicit WriteOnlyStream(std::string& output, DataType type = DataType::INT)
        : sink_type_(SinkType::STRING), output_string_(&output), is_open_(true), data_type_(type) {}

    void Open() { is_open_ = true; }

    void Close() { is_open_ = false; }

    size_t Write(const T& item) {
        if (!is_open_) {
            throw std::runtime_error("Stream is not open");
        }

        switch (sink_type_) {
            case SinkType::SEQUENCE:
                seq_->Append(item);
                break;
            case SinkType::STRING:
                if (!output_string_->empty()) {
                    *output_string_ += " ";
                }
                *output_string_ += Serializer<T>::serialize(item, data_type_);
                break;
        }
        ++position_;
        return position_;
    }

    size_t GetPosition() const { return position_; }
};

#endif