#include "fs.h"
#include "label.h"
#include "tex.h"

int main(int argc, char* argv[]) {
    std::vector<Label> all_labels;
    std::vector<fs::path> tex_paths = tex_files();
    std::vector<TexBuf> tex_bufs = TexBuf::read_files(tex_paths);
    std::vector<TexDataBuf> tdbfs = TexDataBuf::from_bufs(std::move(tex_bufs));

    // for (const TexDataBuf& x : tdbfs) {
    //     for (const Label& label : x.labels()) {
    //         std::cout << label << std::endl;
    //     }
    // }

    // for (const fs::path& fp : tex_files()) {
    //     std::string buf = read_file(fp);
    //     std::istringstream stream(buf);
    //
    //     for (const std::string_view& line : Lines(buf)) {
    //         std::optional<std::string_view> x = parse_label(line);
    //         if (x) {
    //             fwrite(x->data(), x->size(), 1, stdout);
    //             fwrite("\n", 1, 1, stdout);
    //             all_labels.emplace_back(x.value());
    //         }
    //
    //         // if (x.data()) {
    //         //     fwrite(x.data(), x.size(), 1, stdout);
    //         // }
    //         // fwrite(line.data(), line.size(), 1, stdout);
    //         // fwrite("\n", 1, 1, stdout);
    //     }
    // }
    // auto lm = LabelMaker(std::move(all_labels));
    //
    // for (int i = 0; i < 5; i++) {
    //     std::cout << ':' << lm() << std::endl;
    // }
    return 0;
}
