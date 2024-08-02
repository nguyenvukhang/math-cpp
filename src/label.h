#pragma once

#include <functional>
#include <ostream>
#include <string>
#include <unordered_set>

struct Label {
    static const int N = 7;
    char data[N] = {'x', 'x', 'x', 'x', 'x', 'x', 'x'};
    Label() {}

    Label(const std::string_view &value) { strncpy(data, value.data(), 7); }

    // Generate a random label.
    static Label rand();

    // Generate a fresh label, given a set of existing labels.
    // Once found, add to the set of existing labels.
    static Label fresh(std::unordered_set<Label> &existing);

    bool operator==(const Label &x) const { return !strncmp(data, x.data, N); }

    operator std::string() { return std::string(data, N); }

    friend std::ostream &operator<<(std::ostream &os, const Label &obj) {
        os.write(obj.data, N);
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

class LabelMaker {
    std::unordered_set<Label> existing;

   public:
    LabelMaker(std::unordered_set<Label> &&labels)
        : existing(std::move(labels)) {}

    LabelMaker(const std::vector<Label> &&labels)
        : existing(labels.begin(), labels.end()) {}

    // Create a fresh label (never-seen-before), record it, and return it.
    Label operator()() {
        Label::fresh(existing);
        Label l;
        while (!existing.insert(l = Label::rand()).second);
        return l;
    }
};
