#pragma once

#include "fs.h"
#include "label.h"
#include "lines.h"
#include "prelude.h"
#include "string.h"

// Minimal unit you need to interact with one `.tex` file.
class TexBuf {
    fs::path _path;
    std::string _text;

   public:
    TexBuf(const char *p) : _path(p), _text("") {}

    TexBuf(fs::path p) : _path(p), _text("") {};

    // Actually reads the file.
    void read_file() { _text = ::read_file(_path); }

    // Read multiple files.
    static std::vector<TexBuf> read_files(const std::vector<fs::path> &fs);

    // Gather all labels and appends them to the vector referenced.
    void labels(std::vector<Label> &);

    // Iterator over the lines in the `.tex` file.
    Lines lines() const { return Lines(_text); };

    // Move constructor (uncomment when needed)
    // TexBuf(const TexBuf &&src) : _path(src.path()), _text(src._text) {}

    fs::path path() const { return _path; }
    std::string text() const { return _text; }
};

// Everything you want to know about one `.tex` file.
class TexDataBuf : public TexBuf {
    std::vector<Label> _labels;

   public:
    TexDataBuf(const TexBuf &&buf) : TexBuf(buf) {
        std::optional<std::string_view> label;
        for (const std::string_view &line : lines()) {
            if ((label = parse_label(line)))
                _labels.emplace_back(label.value());
        }
    }

    // Read multiple files.
    static std::vector<TexDataBuf> from_bufs(const std::vector<TexBuf> &&bufs);

    std::vector<Label> labels() const { return _labels; }
};
