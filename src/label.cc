#include "label.h"

#include <iostream>
#include <random>

#include "prelude.h"

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

Label Label::rand() {
    Label label;
    label.data[0] = 'a' + rand5(rng);
    for (int i = 1; i < Label::N; i++) label.data[i] = LABEL_CHARS[rand15(rng)];
    return label;
}

Label Label::fresh(std::unordered_set<Label> &existing) {
    Label l;
    while (!existing.insert(l = Label::rand()).second);
    return l;
}

void inspect(const std::unordered_set<Label> &existing) {
    std::cout << '[';
    for (auto &label : existing) std::cout << label << ", ";
    std::cout << ']' << std::endl;
}
