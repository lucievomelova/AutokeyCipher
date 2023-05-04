#ifndef AUTOKEYCIPHER_CTRMODE_H
#define AUTOKEYCIPHER_CTRMODE_H


#include "../Autokey.h"

//counter
class CtrMode : Autokey {
public:
    explicit CtrMode(string key) : Autokey(std::move(key)) {}

    void encrypt(const string &inputFile, const string &outputFile);

    void decrypt(const string &inputFile, const string &outputFile);

    ///create counter string - filled with 0 an on the right end there is counter value
    static string counterString(int counter);
};


#endif //AUTOKEYCIPHER_CTRMODE_H
