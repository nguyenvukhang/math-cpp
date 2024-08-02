#pragma once

#include "prelude.h"

// Reads a file to a `std::string`.
std::string read_file(const fs::path &);

// Writes contents of a `std::string` to a file.
void write_file(const fs::path &, const std::string &);

// Writes newline-spaced lines to a file.
void write_file(const fs::path &, const std::vector<std::string> &);

// Lists `.tex` files recursively at a particular directory.
std::vector<fs::path> tex_files_at(const fs::path &);

// Lists `.tex` files recursively in current directory.
std::vector<fs::path> tex_files();
