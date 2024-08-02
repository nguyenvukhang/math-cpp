#include "prelude.h"

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
            if (n == string::npos) c = s, s = "";
            else c = s.substr(0, n++), s = s.substr(n);
        }
    };

    Iterator begin() { return Iterator(data); }
    Iterator end() { return Iterator(""); }

    const std::string_view data;

    Lines(const std::string_view &data) : data(data) {}
};
