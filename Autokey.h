#include <iostream>
#include <fstream>
#include <utility>
#include <string>
#include <queue>
#include "RandomStringGenerator.h"
#include "CLI.h"

using namespace std;

#ifndef AUTOKEYCIPHER_AUTOKEY_H
#define AUTOKEYCIPHER_AUTOKEY_H

const int BLOCK_SIZE = 64;

class Autokey {
protected:
    string key;

public:
    explicit Autokey(string key)
    {
        this->key = std::move(key);
    }

    void encryptFile(const string& inputFile, const string& outputFile, CipherMode mode);

    void decryptFile(const string& inputFile, const string& outputFile, CipherMode mode);

    ///Encrypt *byte* using *keyByte*
    static char encryptByte(char byte, char keyByte);

    ///Decrypt *byte* using *keyByte*
    static char decryptByte(char encryptedByte, char keyByte);

    string encryptBlock(const string& block);

    string decryptBlock(const string& block);

    static char XOR(char a, char b)
    {
        return (char)(a ^ b);
    }

    static string XorStrings(string a, string b);
};


#endif //AUTOKEYCIPHER_AUTOKEY_H