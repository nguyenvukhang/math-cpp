#include <filesystem>
#include <fstream>
#include <functional>
#include <string>

#include "fs.h"
#include "label.h"

int main(int argc, char* argv[]) {
    auto tx = tex_files();
    for (auto& x : tx) {
        printf("%s\n", x.c_str());
    }
    printf("%d\n", tx.size());
    return 0;
}
