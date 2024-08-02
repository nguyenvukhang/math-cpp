#include "label.h"

#include <iostream>
#include <random>

using namespace std;

std::seed_seq get_random_seed() {
    std::random_device source;
    unsigned int random_data[10];
    for (auto &elem : random_data) elem = source();
    return std::seed_seq(random_data + 0, random_data + 10);
}
static std::uniform_int_distribution<int> rand5(0, 5), rand15(0, 15);
static std::seed_seq seed = get_random_seed();
static std::default_random_engine rng(seed);

const char LABEL_CHARS[] = {'a', 'b', 'c', 'd', 'e', 'f', '0', '1',
                            '2', '3', '4', '5', '6', '7', '8', '9'};

string Label::to_tex() {
    char result[16] = "\\label{_______}";
    strncpy(&result[7], data, 7);
    return result;
}

Label Label::rand() {
    Label label;
    label.data[0] = 'a' + rand5(rng);
    for (int i = 1; i < 7; i++) label.data[i] = LABEL_CHARS[rand15(rng)];
    return label;
}

Label Label::fresh(std::unordered_set<Label> &existing) {
    Label fresh = Label::rand();
    while (existing.contains(fresh)) {
        fresh = Label::rand();
    }
    existing.insert(fresh);
    return fresh;
}

void inspect(const std::unordered_set<Label> &existing) {
    cout << '[';
    for (auto &label : existing) cout << label << ", ";
    cout << ']' << endl;
}
