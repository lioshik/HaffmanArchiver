#include "BitsReader.h"

#include <algorithm>

BitsReader::BitsReader(std::istream &stream, bool is_little_endian)
    : input_stream_(stream), read_little_endian_(is_little_endian), current_byte_(0), pos_in_byte_(8) {
}

bool BitsReader::NextBit() {
    if (pos_in_byte_ >= 8) {
        input_stream_.get(reinterpret_cast<char &>(current_byte_));
        pos_in_byte_ = 0;
    }
    if (input_stream_.eof()) {
        return false;
    }
    return current_byte_ & (1 << (7 - (pos_in_byte_++)));
}

std::vector<bool> BitsReader::ReadBits(size_t count) {
    std::vector<bool> bits;
    bits.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        bits.push_back(NextBit());
    }
    if (read_little_endian_) {
        std::reverse(bits.begin(), bits.end());
    }
    return bits;
}

bool BitsReader::IsEof() const {
    return input_stream_.eof();
}

void BitsReader::Reset() {
    input_stream_.clear();
    input_stream_.seekg(0);
    current_byte_ = 0;
    pos_in_byte_ = 8;
}
