#include <filesystem>
#include <fstream>
#include <functional>
#include <string>

#include "fs.h"
#include "label.h"

int main(int argc, char *argv[]) {
    printf("HI\n");
    string buf = read_file("lib/uatfo/mod.tex");
    printf("%s\n", buf.c_str());
    return 0;
}
