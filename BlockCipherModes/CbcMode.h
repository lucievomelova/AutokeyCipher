#ifndef AUTOKEYCIPHER_CBCMODE_H
#define AUTOKEYCIPHER_CBCMODE_H

#include "../Autokey.h"

//cipher block chaining
class CbcMode : Autokey {
public:
    explicit CbcMode(string key) : Autokey(std::move(key)) {}

    void encrypt(const string &inputFile, const string &outputFile);

    void decrypt(const string &inputFile, const string &outputFile);
};


#endif //AUTOKEYCIPHER_CBCMODE_H
