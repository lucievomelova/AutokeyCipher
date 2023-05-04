#ifndef AUTOKEYCIPHER_ECBMODE_H
#define AUTOKEYCIPHER_ECBMODE_H

#include <utility>
#include "../Autokey.h"

//electronic code book
class EcbMode : Autokey {

public:
    explicit EcbMode(string key) : Autokey(std::move(key)) {}

    void encrypt(const string &inputFile, const string &outputFile);

    void decrypt(const string &inputFile, const string &outputFile);
};


#endif //AUTOKEYCIPHER_ECBMODE_H