#pragma once

#include <string_view>
#include <vector>

struct Mark {
    using str = std::string_view;

    const str kind, title, sha;
    const unsigned int lnum;
    const std::vector<str> content;

    Mark(str kind, str title, str sha, unsigned int lnum,
         std::vector<str> &&content)
        : kind(kind), title(title), sha(sha), lnum(lnum), content(content) {}
};
