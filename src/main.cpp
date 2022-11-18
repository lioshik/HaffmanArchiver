#include "Archiver.h"
#include <iostream>

void PrintHelpMessage() {
    std::cout << "-h for help\n"
                 "-c <archive_file> <file1>[<file2>...] Compress archive\n"
                 "-d <archive_file> Decompress archive\n";
}

int main(int argc, char* argv[]) {
    std::vector<std::string> arguments;
    for (int i = 1; i < argc; ++i) {
        arguments.push_back(argv[i]);
    }
    if (arguments.size() >= 3 && arguments[0] == "-c") {
        std::vector<std::string> files_to_compress;
        for (size_t i = 2; i < arguments.size(); ++i) {
            files_to_compress.push_back(arguments[i]);
        }
        Archiver archiver;
        try {
            archiver.CompressFiles(files_to_compress, arguments[1]);
        } catch (std::runtime_error& e) {
            std::cout << "ERROR: " << e.what();
            return 111;
        }
    } else if (arguments.size() == 2 && arguments[0] == "-d") {
        Archiver archiver;
        try {
            archiver.DecompressArchive(arguments[1]);
        } catch (std::runtime_error& e) {
            std::cout << "ERROR: " << e.what();
            return 111;
        }
    } else {
        PrintHelpMessage();
    }
    return 0;
}
