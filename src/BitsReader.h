#pragma once

#include <istream>
#include <vector>

class BitsReader {
public:
    BitsReader(std::istream& stream, bool is_little_endian);

    bool NextBit();
    std::vector<bool> ReadBits(size_t count);

    bool IsEof() const;

    void Reset();

private:
    std::istream& input_stream_;
    bool read_little_endian_;
    uint8_t current_byte_;
    size_t pos_in_byte_;
};
