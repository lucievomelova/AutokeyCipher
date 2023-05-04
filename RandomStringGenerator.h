#ifndef AUTOKEYCIPHER_RANDOMSTRINGGENERATOR_H
#define AUTOKEYCIPHER_RANDOMSTRINGGENERATOR_H

#include <iostream>
#include <random>

using namespace std;


class RandomStringGenerator {
private:

public:
    static string generate(size_t length);

};


#endif //AUTOKEYCIPHER_RANDOMSTRINGGENERATOR_H
