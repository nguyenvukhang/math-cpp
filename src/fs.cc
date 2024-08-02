#include "fs.h"

string read_file(const fs::path &path) {
    std::ifstream stream(path);
    std::stringstream buffer;
    buffer << stream.rdbuf();
    return buffer.str();
}

void write_file(const fs::path &path, const string &text) {
    std::ofstream stream(path);
    stream << text;
    stream.close();
}

void write_file(const fs::path &path, const std::vector<string> &lines) {
    std::ofstream stream(path);
    for (auto &line : lines) stream << line << '\n';
    stream.close();
}

void tex_files_at(const fs::path &dir, std::vector<fs::path> &acc) {
    DIR *dir_fd = opendir(dir.c_str());
    if (!dir_fd) return perror("opendir");
    struct dirent *e;
    fs::path path;
    while ((e = readdir(dir_fd)) != NULL) {
#define _(x, n) \
    if (!strncmp(e->d_name, x, n)) continue;
#define endswith(x) \
    !strncmp(e->d_name + e->d_namlen - sizeof(x) + 1, x, sizeof(x) - 1)
        // skip `.` and `..` for obvious reasons
        _(".", 1) _("..", 2);
        path = dir.compare(".") == 0 ? e->d_name : dir / e->d_name;
        if (e->d_type == DT_DIR) {
            _(".git", 4) _(".build", 6) _("target", 6) _(".github", 7);
            tex_files_at(path, acc);
        } else if (e->d_type == DT_REG && endswith(".tex")) {
            acc.emplace_back(path);
        }
#undef _
#undef endswith
    }
    closedir(dir_fd);
}

std::vector<fs::path> tex_files_at(const fs::path &dir) {
    std::vector<fs::path> vec;
    tex_files_at(dir, vec);
    return vec;
}

std::vector<fs::path> tex_files() { return tex_files_at("."); }
