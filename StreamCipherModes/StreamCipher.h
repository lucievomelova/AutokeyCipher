#ifndef AUTOKEYCIPHER_STREAMCIPHER_H
#define AUTOKEYCIPHER_STREAMCIPHER_H


#include "../Autokey.h"

//normal autokey cipher, whole file is processed at once
class StreamCipher : Autokey {
public:
    explicit StreamCipher(string key) : Autokey(std::move(key)) {}

    void encrypt(const string &inputFile, const string &outputFile);

    void decrypt(const string &inputFile, const string &outputFile);
};


#endif //AUTOKEYCIPHER_STREAMCIPHER_H
