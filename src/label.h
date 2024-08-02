#pragma once

#include <functional>
#include <ostream>
#include <string>
#include <unordered_set>

struct Label {
    char data[7] = {'x', 'x', 'x', 'x', 'x', 'x', 'x'};
    Label() {}

    Label(const std::string_view &value) { strncpy(data, value.data(), 7); }

    // Convert to LaTeX form.
    //
    // "d5f_ab7d" → "\label{d5f_ab7d}"
    std::string to_tex();

    // Generate a random label.
    static Label rand();

    // Generate a fresh label, given a set of existing labels.
    // Once found, add to the set of existing labels.
    static Label fresh(std::unordered_set<Label> &existing);

    bool operator==(const Label &x) const { return !strncmp(data, x.data, 7); }

    operator std::string() { return std::string(data, 7); }

    friend std::ostream &operator<<(std::ostream &os, const Label &obj) {
        os.write(obj.data, 7);
        return os;
    }
};

namespace std {
template <>
struct hash<Label> {
    std::size_t operator()(const Label &label) const noexcept {
        return std::hash<std::string_view>()(label.data);
    }
};
}  // namespace std
