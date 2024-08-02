#include "fs.h"

string read_file(const fs::path &path) {
    std::ifstream stream(path);
    std::stringstream buffer;
    buffer << stream.rdbuf();
    return buffer.str();
}

void write_file(const fs::path &path, const string &text) {
    std::ofstream stream(path);
    stream << text;
    stream.close();
}

void write_file(const fs::path &path, const std::vector<string> &lines) {
    std::ofstream stream(path);
    for (auto &line : lines) stream << line << '\n';
    stream.close();
}
