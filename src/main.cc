#include "fs.h"
#include "label.h"
#include "lines.h"
#include "prelude.h"
#include "string.h"

int main(int argc, char* argv[]) {
    for (const fs::path& fp : tex_files()) {
        std::string buf = read_file(fp);
        std::istringstream stream(buf);

        for (const std::string_view& line : Lines(buf)) {
            std::optional<std::string_view> x = parse_label(line);
            if (x) {
                fwrite(x->data(), x->size(), 1, stdout);
                fwrite("\n", 1, 1, stdout);
            }
            // if (x.data()) {
            //     fwrite(x.data(), x.size(), 1, stdout);
            // }
            // fwrite(line.data(), line.size(), 1, stdout);
            // fwrite("\n", 1, 1, stdout);
        }
    }
    return 0;
}
