#include "fs.h"
#include "label.h"
#include "tex.h"

int main(int argc, char* argv[]) {
    std::vector<fs::path> tex_paths = tex_files();
    std::vector<TexBuf> tex_bufs = TexBuf::read_files(tex_paths);
    std::vector<TexDataBuf> tdbfs = TexDataBuf::from_bufs(std::move(tex_bufs));

    // collect all labels
    std::vector<Label> all_labels;
    for (const TexDataBuf& x : tdbfs)
        for (const Label& label : x.labels()) all_labels.emplace_back(label);

    LabelMaker lm(std::move(all_labels));

    for (const TexDataBuf& x : tdbfs) {
        for (auto line : x.lines()) {
            if (needs_label(line)) {
                std::cout << line << "\\label{" << lm() << '}' << std::endl;
            }
        }
    }
    return 0;
}
