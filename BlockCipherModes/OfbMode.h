#ifndef AUTOKEYCIPHER_OFBMODE_H
#define AUTOKEYCIPHER_OFBMODE_H

#include "../Autokey.h"

//output feedback
class OfbMode : Autokey {
public:
    explicit OfbMode(string key) : Autokey(std::move(key)) {}

    void encrypt(const string &inputFile, const string &outputFile);

    void decrypt(const string &inputFile, const string &outputFile);
};


#endif //AUTOKEYCIPHER_OFBMODE_H
