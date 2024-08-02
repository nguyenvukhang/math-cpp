#pragma once

#include <cassert>
#include <dirent.h>
#include <fcntl.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <stdio.h>
#include <string>
#include <sys/stat.h>
#include <thread>
#include <unistd.h>
#include <unordered_set>
#include <vector>

#include "label.h"

using namespace std;
namespace fs = filesystem;

// An iterator over lines of a string.
class Lines {
public:
  struct Iterator {
    Iterator(std::string_view buffer) : s(buffer) { next(); }

    std::string_view &operator*() { return c; }
    std::string_view *operator->() { return &c; }
    friend bool operator==(Iterator &a, Iterator &b) {
      return a.c == b.c && a.s == b.s;
    };
    friend bool operator!=(Iterator &a, Iterator &b) { return !(a == b); };

    Iterator &operator++() {
      next();
      return *this;
    }

    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

  private:
    std::string_view s, c;

    void next() {
      size_t n = s.find('\n');
      if (n == string::npos)
        c = s, s = "";
      else
        c = s.substr(0, n++), s = s.substr(n);
    }
  };

  Iterator begin() { return Iterator(data); }
  Iterator end() { return Iterator(""); }

  const std::string_view data;

  Lines(const std::string_view &data) : data(data) {}
};

// Subcommands, one for each cli function.
enum Subcommand {
  BUILD_TEX = 0,
  BUILD_PDF,
  ONE_SHOT_JOB,
  LABEL,
  NO_COMMAND,
  UNRECOGNIZED_COMMAND
};

struct Job {
  fs::path root_file;
  string jobname;
  vector<string> packages;

  Job(fs::path &&root_file, string &&jobname, const vector<string> &packages)
      : root_file(root_file), jobname(jobname), packages(packages) {}
};

// Abstract class for things that can write to buffers.
struct Write {
  virtual void write(const std::string &text) = 0;
  virtual void close() = 0;
  void writeln(const std::string &text) {
    write(text);
    write("\n");
  }
  void add_packages(const vector<string> &packages) {
    for (auto &p : packages) {
      write("\\usepackage{");
      write(p);
      writeln("}");
    }
  }
};

struct PdfTex : Write {
  PdfTex(const string &jobname);
  virtual void write(const std::string &text);
  virtual void close() { wait(); };
  void wait();

private:
  int pdftex_pid;
  // .0: read-end, .1: write-end
  // (p: STDIN, q: STDOUT)
  int p[2], q[2];
  thread printer_thread_handle;
};

struct File : Write {
  File(const fs::path &filename);
  virtual void write(const std::string &text);
  virtual void close() { stream.close(); };

private:
  ofstream stream;
};

// Read a file into a `string`.
string read_file(const fs::path &path);

// Write to a file with a `string`.
void write_file(const fs::path &path, const string &text);

// Write to a file with a list of lines.
// Lines themselves should NOT contains newline chars.
void write_file(const fs::path &path, const vector<string> &lines);

// Ensures that a directory exists at `path`.
void ensure_dir(const fs::path &path);

// List all `*.tex` files recursively under `dir`.
vector<fs::path> tex_files_at(const fs::path &dir);

// List all `*.tex` files recursively under current dir.
vector<fs::path> tex_files();

// Parse `\use{<relative file>}`. If not found, `line` will be unchanged.
fs::path *parse_import(string &line);

// Try to resolve `fp` to a `.tex` file.
fs::path *resolve(fs::path &fp);

// Everything you need to interact with one `.tex` file.
struct TexBuf {
  TexBuf(const char *p) : _path(p), _text("") {}

  TexBuf(fs::path p) : _path(p), _text("") {};

  // Actually reads the file.
  void read_file() { _text = ::read_file(_path); }

  // Read multiple files.
  static vector<TexBuf> read_files(const vector<fs::path> &fs);

  // Gather all labels and appends them to the vector referenced.
  void labels(vector<Label> &);

  Lines lines() { return Lines(_text); };

  // Move constructor
  TexBuf(TexBuf &&src) : _text("") {
    _text.swap(src._text);
    swap(_path, src._path);
  }

  fs::path path() const { return _path; }
  string text() const { return _text; }

private:
  fs::path _path;
  string _text;
};

struct Project {
  Project() {}
  Project(vector<TexBuf> &texbufs) {
    for (auto &t : texbufs)
      _texbufs.push_back(&t);
  }

  void add_texbuf(TexBuf &t) { _texbufs.push_back(&t); }

  // Gather all labels in `texbufs` WITHOUT checking for uniqueness.
  vector<Label> labels() const;

  vector<TexBuf *> texbufs() const { return _texbufs; }

private:
  vector<TexBuf *> _texbufs;
};

Subcommand parse_subcommand(const string &arg);

bool has_ext(char *x, const char *ext);
