#include <string>

#include "fs.h"
#include "label.h"
#include "lines.h"
#include "prelude.h"

int main(int argc, char* argv[]) {
    for (const fs::path& fp : tex_files()) {
        std::string buf = read_file(fp);
        std::istringstream stream(buf);

        for (const std::string_view& line : Lines(buf)) {
            fwrite(line.data(), line.size(), 1, stdout);
            fwrite("\n", 1, 1, stdout);
        }

        break;
    }
    return 0;
}
