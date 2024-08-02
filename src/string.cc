#include "string.h"

std::optional<std::string_view> parse_label(const std::string_view& line) {
    size_t idx = line.find("\\label{");
    if (idx == string::npos) return std::nullopt;
    std::string_view rem = line.substr(idx + 7);
    if ((idx = rem.find('}')) == string::npos) return std::nullopt;
    return rem.substr(0, idx);
}
