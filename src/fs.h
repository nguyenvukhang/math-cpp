#include <fstream>
#include <sstream>

#include "prelude.h"

std::string read_file(const fs::path &);

void write_file(const fs::path &, const std::string &);

void write_file(const fs::path &, const std::vector<std::string> &);
