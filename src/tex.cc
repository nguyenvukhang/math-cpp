#include "tex.h"

using TDB = TexDataBuf;

std::vector<TexBuf> TexBuf::read_files(const std::vector<fs::path> &fs) {
    std::vector<TexBuf> texbufs;
    texbufs.reserve(fs.size());
    auto build = [](fs::path f) {
        TexBuf tb(f);
        tb.read_file();
        return tb;
    };
    transform(make_move_iterator(fs.begin()), make_move_iterator(fs.end()),
              back_inserter(texbufs), build);
    return texbufs;
}

std::vector<TDB> TDB::from_bufs(const std::vector<TexBuf> &&bufs) {
    std::vector<TDB> tdbfs;
    tdbfs.reserve(bufs.size());
    auto build = [](TexBuf f) { return TexDataBuf(std::move(f)); };
    transform(make_move_iterator(bufs.begin()), make_move_iterator(bufs.end()),
              back_inserter(tdbfs), build);
    return tdbfs;
}
