#include "RandomStringGenerator.h"

string RandomStringGenerator::generate(size_t length) {
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, 255);

    std::string random_string;

    for (std::size_t i = 0; i < length; ++i) {
        random_string += (char) (distribution(generator));
    }
    return random_string;
}
