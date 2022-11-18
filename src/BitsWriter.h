#pragma once

#include <ostream>
#include <vector>

class BitsWriter {
public:
    explicit BitsWriter(std::ostream& write_stream, bool is_little_endian);
    void WriteBits(std::vector<bool> data);
    void CloseStream();

private:
    std::ostream& output_stream_;
    bool write_little_endian_ = false;
    uint8_t current_byte_ = 0;
    size_t pos_in_byte_ = 0;
};
