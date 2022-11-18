#include "Archiver.h"
#include "BitsReader.h"
#include "HaffmanAlgorithm.h"

#include <algorithm>
#include <fstream>

void Archiver::CompressFiles(const std::vector<std::string>& file_paths, std::string output_path) const {
    std::ofstream output_stream(output_path);
    if (output_stream.bad() || !output_stream.is_open()) {
        throw std::runtime_error("can't open file to write");
    }
    BitsWriter writer(output_stream, false);
    for (size_t i = 0; i < file_paths.size(); ++i) {
        if (i == file_paths.size() - 1) {
            CompressNextFile(file_paths[i], writer, true);
        } else {
            CompressNextFile(file_paths[i], writer, false);
        }
    }
    writer.CloseStream();
}

void Archiver::DecompressArchive(const std::string& filepath) const {
    std::ifstream output_stream(filepath);
    if (!output_stream.is_open() || output_stream.bad()) {
        throw std::runtime_error("Can't read file");
    }

    BitsReader reader(output_stream, false);

    bool archive_end = false;
    while (!archive_end) {
        size_t total_symbols_count = ByteToNumber(reader.ReadBits(9));
        std::vector<std::vector<bool>> symbols;
        for (size_t i = 0; i < total_symbols_count; ++i) {
            symbols.push_back(reader.ReadBits(9));
        }

        std::vector<std::pair<size_t, std::vector<bool>>> lengths;
        size_t symbol_index = 0;
        size_t length = 0;
        size_t current_count = 0;

        while (symbol_index < total_symbols_count) {
            while (current_count == 0) {
                current_count = ByteToNumber(reader.ReadBits(9));
                ++length;
            }
            lengths.emplace_back(length, symbols[symbol_index]);
            ++symbol_index;
            --current_count;
        }

        auto code_by_symbol = BuildCanonicalDictFromLengths(lengths);
        std::unordered_map<std::vector<bool>, std::vector<bool>> codes;
        for (const auto& [symbol, code] : code_by_symbol) {
            codes[code] = symbol;
        }

        std::vector<bool> current_code;
        std::string output_filename;
        while (codes.find(current_code) == codes.end() || codes[current_code] != FILENAME_END) {
            while (codes.find(current_code) == codes.end()) {
                bool bit = reader.NextBit();
                current_code.push_back(bit);
            }
            if (codes[current_code] != FILENAME_END) {
                output_filename.push_back(static_cast<char>(ByteToNumber(codes[current_code])));
                current_code.clear();
            } else {
                break;
            }
        }
        std::ofstream file_output(output_filename);
        if (!file_output.is_open() || file_output.bad()) {
            throw std::runtime_error("Can't open file to write");
        }
        BitsWriter writer(file_output, false);
        current_code.clear();
        while (codes.find(current_code) == codes.end() ||
               (codes[current_code] != ARCHIVE_END && codes[current_code] != ONE_MORE_FILE)) {
            while (codes.find(current_code) == codes.end()) {
                bool bit = reader.NextBit();
                current_code.push_back(bit);
            }
            if (codes[current_code] != ARCHIVE_END && codes[current_code] != ONE_MORE_FILE) {
                auto code = codes[current_code];
                code.erase(code.begin());
                writer.WriteBits(code);
                current_code.clear();
            } else {
                break;
            }
        }
        writer.CloseStream();
        if (codes[current_code] == ARCHIVE_END) {
            archive_end = true;
        }
    }
}

void Archiver::CompressNextFile(const std::string& file_path, BitsWriter& writer, bool is_last_file) const {
    std::ifstream file_stream(file_path);
    if (file_stream.bad() || !file_stream.is_open()) {
        throw std::runtime_error("Can't open file for reading");
    }

    size_t last_slash = file_path.rfind('/');
    std::string file_name = file_path;
    if (last_slash != std::string::npos) {
        file_name = file_path.substr(last_slash + 1);
    }
    BitsReader reader(file_stream, false);

    std::unordered_map<std::vector<bool>, size_t> symbol_frequency;
    symbol_frequency[FILENAME_END] = 1;
    symbol_frequency[ARCHIVE_END] = 1;
    symbol_frequency[ONE_MORE_FILE] = 1;

    std::vector<std::vector<bool>> filename_bytes;
    for (uint8_t symbol : file_name) {
        std::vector<bool> symbol_byte(9);
        size_t i = 0;
        while (symbol > 0) {
            symbol_byte[8 - i] = symbol % 2;
            symbol /= 2;
            ++i;
        }
        filename_bytes.push_back(symbol_byte);
        ++symbol_frequency[symbol_byte];
    }

    while (!reader.IsEof()) {
        auto next_byte = reader.ReadBits(8);
        if (reader.IsEof()) {
            break;
        }
        next_byte.insert(next_byte.begin(), false);
        ++symbol_frequency[next_byte];
    }

    HaffmanAlgorithm haffman_algorithm(symbol_frequency);
    auto canonical_dict = haffman_algorithm.GetCanonicalDict();

    std::unordered_map<size_t, size_t> length_count;
    size_t max_symbol_code_size = 0;
    for (const auto& [symbol, code] : canonical_dict) {
        max_symbol_code_size = std::max(code.size(), max_symbol_code_size);
        ++length_count[code.size()];
    }

    writer.WriteBits(NumberToByte(canonical_dict.size()));
    for (const auto& alphabet_symbol : haffman_algorithm.GetOrderedAlphabet()) {
        writer.WriteBits(alphabet_symbol);
    }
    for (size_t length = 1; length <= max_symbol_code_size; ++length) {
        writer.WriteBits(NumberToByte(length_count[length]));
    }

    for (const auto& filename_byte : filename_bytes) {
        writer.WriteBits(canonical_dict[filename_byte]);
    }
    writer.WriteBits(canonical_dict[FILENAME_END]);

    reader.Reset();
    while (!reader.IsEof()) {
        auto symbol = reader.ReadBits(8);
        if (reader.IsEof()) {
            break;
        }
        symbol.insert(symbol.begin(), false);
        if (canonical_dict.find(symbol) == canonical_dict.end()) {
            throw std::logic_error("No haffman code for this symbol");
        }
        writer.WriteBits(canonical_dict[symbol]);
    }
    if (is_last_file) {
        writer.WriteBits(canonical_dict[ARCHIVE_END]);
    } else {
        writer.WriteBits(canonical_dict[ONE_MORE_FILE]);
    }
}

std::vector<bool> Archiver::NumberToByte(size_t number) const {
    std::vector<bool> result(9, false);
    size_t i = 0;
    while (number > 0) {
        result[8 - i] = (number % 2);
        number /= 2;
        ++i;
    }
    return result;
}

size_t Archiver::ByteToNumber(const std::vector<bool>& byte) const {
    size_t result = 0;
    for (size_t i = 0; i < byte.size(); ++i) {
        result *= 2;
        if (byte[i]) {
            ++result;
        }
    }
    return result;
}

std::unordered_map<std::vector<bool>, std::vector<bool>> Archiver::BuildCanonicalDictFromLengths(
    const std::vector<std::pair<size_t, std::vector<bool>>>& lengths) const {
    std::unordered_map<std::vector<bool>, std::vector<bool>> result;

    std::vector<bool> current_code;
    for (const auto& [length, data] : lengths) {
        size_t i = current_code.size();
        while (i > 0 && current_code[i - 1]) {
            current_code[i - 1] = false;
            --i;
        }
        if (i > 0) {
            current_code[i - 1] = true;
        }
        while (current_code.size() < length) {
            current_code.push_back(false);
        }

        result[data] = current_code;
    }

    return result;
}
