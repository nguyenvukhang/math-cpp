#include "cli.h"
#include "consts.h"
#include "label.h"
#include "prelude.h"
#include "str.h"

void walk(const fs::path &fp, function<void(string &)> f) {
    ifstream file(fp);
    fs::path *parsed;
    for (string line; getline(file, line);) {
        parsed = parse_import(line);
        if (parsed == NULL) {
            f(line);
            continue;
        }
        *parsed = fp.parent_path() / *parsed;
        parsed = resolve(*parsed);
        if (parsed == NULL) {
            f(line);
            continue;
        }
        walk(*parsed, f);
    }
}

// Generate labels for an entire file of text.
// Returns the new edited lines in a std::vector<std::string>.
vector<string> generate_labels(const string &text, bool &dirty,
                               function<Label()> mk_label) {
    vector<string> newlines;
    // istringstream buffer(text);
    // for (string line; std::getline(buffer, line);) {
    //   if (line.size() <= 1) {
    //     newlines.emplace_back(line);
    //     continue;
    //   }
    //   str line_view(line);
    //   line_view = line_view.substr(1);
    //   auto mark = line_view.starts_with_mark();
    //
    //   bool starts_with_mark = false;
    //   for (auto &m : MARKS) {
    //     if (line_view.starts_with(m)) {
    //       starts_with_mark = true;
    //       break;
    //     }
    //   }
    //
    //   if (starts_with_mark && line_view.find("\\label{") != string::npos) {
    //     line += "\\label{";
    //     line += mk_label();
    //     line += '}';
    //     dirty = true;
    //     newlines.emplace_back(line);
    //     continue;
    //   }
    //   newlines.emplace_back(line);
    // }
    return newlines;
}

// labels all `.tex` files under (recursively) the current working
// directory.
void label() {
    vector<TexBuf> tbfs = TexBuf::read_files(tex_files());
    Project proj(tbfs);
    vector<Label> all_labels = proj.labels();
    unordered_set<Label> existing(all_labels.begin(), all_labels.end());
    auto label_maker = [&]() { return Label::fresh(existing); };

    vector<string> newlines;
    for (auto &tb : tbfs) {
        bool dirty = false;
        newlines = generate_labels(tb.text(), dirty, label_maker);
        if (!dirty) continue;
        ofstream outfile(tb.path());
        cout << "DIRTY: " << tb.path() << endl;
        write_file(tb.path(), newlines);
    }
}

int __main__(int argc, char *argv[]) {
    Cli cli = parse(argc, argv);

    if (cli.add_labels) {
        label();
        return EXIT_SUCCESS;
    }

    // Initialize writer.
    Write *w;
    switch (cli.output_type) {
        case OutputType::O_PDF:
            w = new PdfTex(cli.jobname);
            break;
        case OutputType::O_TEX:
            w = new File(cli.output_file);
            break;
    }

    // Write opening boilerplate
    if (cli.add_default_headers) {
        w->writeln("\\documentclass[oneside]{book}");
        w->writeln("\\usepackage{headers}");
        w->writeln("\\begin{document}");
    }

    // Main build
    walk(cli.root_file, [&](string &line) { w->writeln(line); });

    // Write closing boilerplate
    if (cli.add_default_headers) {
        w->writeln("\\end{document}");
    }

    w->close();
    free(w);
    cout << "Output sent to: " << cli.output_file << endl;
    fs::path &f = cli.output_file;
    rename(f.c_str(), ("/Users/khang/repos/math/cpp" / f.filename()).c_str());
    return EXIT_SUCCESS;
}

// Strips the {} out from both ends of `src`. Overwrites `src`. Does
// nothing if `src` isn't surrounded in braces.
bool strip_braces(string_view &src) {
    if (src.starts_with('{') && src.ends_with('}')) {
        src = src.substr(1, src.length() - 2);
        return true;
    }
    return false;
}

// Splits `src` by "\label". Overwrites `src` with the LHS, and
// `label_sha` will contain only the SHA found in the label.
bool split_by_label(str &src, str &label_sha) {
    size_t n = src.find("\\label");
    if (n == string::npos) return false;
    label_sha = src.substr(n + 6);
    if (strip_braces(label_sha)) {
        src = src.substr(0, n);
    }
    return true;
}

// Splits `src` by mark. Overwrites `src` with the mark found, and
// `rem` will contain the remaining part of the string_view.
bool split_by_mark(str &src, str &rem) {
    for (auto &m : MARKS) {
        if (src.starts_with(m)) {
            rem = src.substr(m.length()), src = m;
            return true;
        }
    }
    return false;
}

bool parse_mark(const str &src, str &kind, str &title, str &sha) {
    if (!src.starts_with('\\')) return false;
    kind = src.substr(1);
    return split_by_mark(kind, title) && split_by_label(title, sha) &&
           strip_braces(title);
}

void parse_marks(Lines &lines) {
    int lnum = 0;
    for (auto &x : lines) {
        lnum++;
    }
}

const char *RED_SEMI = "\x1b[0;31m;;\x1b[0m";

void debugging() {
    TexBuf tb(
        "/Users/khang/repos/math/_/nonlinear_optimization_constrained.tex");
    tb.read_file();
    for (str line : tb.lines()) {
        if (line.length() == 0) continue;
        str kind, title, label;
        if (parse_mark(line, kind, title, label)) {
            cout << kind << RED_SEMI << title << RED_SEMI << label << endl;
        }
    }
}

int main(int argc, char *argv[]) {
    const bool debug_mode = true;
    if (debug_mode) {
        debugging();
    } else {
        __main__(argc, argv);
    }
    return 0;
}
