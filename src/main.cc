#include "fs.h"
#include "label.h"
#include "lines.h"
#include "prelude.h"
#include "string.h"

class LabelMaker {
    std::unordered_set<Label> existing;

   public:
    LabelMaker(const std::vector<Label>&& labels)
        : existing(labels.begin(), labels.end()) {}

    // Create a fresh label (never-seen-before), and return it.
    Label fresh() {
        Label l;
        while (!existing.insert(l = Label::rand()).second);
        return l;
    }
};

int main(int argc, char* argv[]) {
    std::vector<Label> all_labels;

    for (const fs::path& fp : tex_files()) {
        std::string buf = read_file(fp);
        std::istringstream stream(buf);

        for (const std::string_view& line : Lines(buf)) {
            std::optional<std::string_view> x = parse_label(line);
            if (x) {
                fwrite(x->data(), x->size(), 1, stdout);
                fwrite("\n", 1, 1, stdout);
                all_labels.emplace_back(x.value());
            }

            // if (x.data()) {
            //     fwrite(x.data(), x.size(), 1, stdout);
            // }
            // fwrite(line.data(), line.size(), 1, stdout);
            // fwrite("\n", 1, 1, stdout);
        }
    }
    LabelMaker lm(std::move(all_labels));

    for (int i = 0; i < 5; i++) {
        std::cout << ':' << lm.fresh() << std::endl;
    }
    return 0;
}
