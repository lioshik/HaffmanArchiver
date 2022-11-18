#include "BitsWriter.h"

#include <algorithm>

BitsWriter::BitsWriter(std::ostream& write_stream, bool is_little_endian)
    : output_stream_(write_stream), write_little_endian_(is_little_endian) {
}

void BitsWriter::WriteBits(std::vector<bool> data) {
    if (write_little_endian_) {
        std::reverse(data.begin(), data.end());
    }

    for (const auto& bit : data) {
        if (bit) {
            current_byte_ |= (1ll << (7 - pos_in_byte_));
        }
        ++pos_in_byte_;

        if (pos_in_byte_ == 8) {
            output_stream_.put(current_byte_);
            current_byte_ = 0;
            pos_in_byte_ = 0;
        }
    }
    output_stream_.flush();
}

void BitsWriter::CloseStream() {
    if (pos_in_byte_ > 0) {
        output_stream_.put(current_byte_);
    }
    output_stream_.flush();
}
