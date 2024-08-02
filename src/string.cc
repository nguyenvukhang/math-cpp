#include "string.h"

std::optional<std::string_view> parse_label(const std::string_view& line) {
    size_t idx = line.find("\\label{");
    if (idx == string::npos) return std::nullopt;
    std::string_view rem = line.substr(idx + 7);
    if ((idx = rem.find('}')) == string::npos) return std::nullopt;
    return rem.substr(0, idx);
}

bool is_marked_line(const std::string_view& line) {
    if (line[0] != '\\') return false;
    std::string_view post = line.substr(1);
    for (const std::string& mark : MARKS) {
        if (post.starts_with(mark)) return true;
    }
    return false;
}

bool needs_label(const std::string_view& line) {
    return line.find("\\label") == string::npos &&
           (line.starts_with("\\subsection") || is_marked_line(line));
}
