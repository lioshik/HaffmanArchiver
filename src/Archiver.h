#pragma once

#include "BitsWriter.h"

#include <string>
#include <unordered_map>
#include <vector>

const std::vector<bool> FILENAME_END = {true, false, false, false, false, false, false, false, false};
const std::vector<bool> ONE_MORE_FILE = {true, false, false, false, false, false, false, false, true};
const std::vector<bool> ARCHIVE_END = {true, false, false, false, false, false, false, true, false};

class Archiver {
public:
    void CompressFiles(const std::vector<std::string>& file_paths, std::string output_path) const;
    void DecompressArchive(const std::string& filepath) const;

private:
    void CompressNextFile(const std::string& file_path, BitsWriter& writer, bool is_last_file) const;
    std::vector<bool> NumberToByte(size_t number) const;
    size_t ByteToNumber(const std::vector<bool>& byte) const;

    std::unordered_map<std::vector<bool>, std::vector<bool>> BuildCanonicalDictFromLengths(
        const std::vector<std::pair<size_t, std::vector<bool>>>& lengths) const;
};
