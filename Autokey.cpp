#include "Autokey.h"
#include "BlockCipherModes/EcbMode.h"
#include "BlockCipherModes/CbcMode.h"
#include "BlockCipherModes/OfbMode.h"
#include "BlockCipherModes/CtrMode.h"
#include "StreamCipherModes/StreamCipher.h"

void Autokey::encryptFile(const string &inputFile, const string &outputFile, CipherMode mode) {
    switch (mode) {
        case ECB: {
            EcbMode ecb(key);
            ecb.encrypt(inputFile, outputFile);
            break;
        }
        case CBC: {
            CbcMode cbc(key);
            cbc.encrypt(inputFile, outputFile);
            break;
        }
        case OFB: {
            OfbMode ofb(key);
            ofb.encrypt(inputFile, outputFile);
            break;
        }
        case CTR: {
            CtrMode ctr(key);
            ctr.encrypt(inputFile, outputFile);
            break;
        }
        case stream: {
            StreamCipher stream(key);
            stream.encrypt(inputFile, outputFile);
            break;
        }
    }
}

void Autokey::decryptFile(const string &inputFile, const string &outputFile, CipherMode mode) {
    switch (mode) {
        case ECB: {
            EcbMode ecb(key);
            ecb.decrypt(inputFile, outputFile);
            break;
        }
        case CBC: {
            CbcMode cbc(key);
            cbc.decrypt(inputFile, outputFile);
            break;
        }
        case OFB: {
            OfbMode ofb(key);
            ofb.decrypt(inputFile, outputFile);
            break;
        }
        case CTR: {
            CtrMode ctr(key);
            ctr.decrypt(inputFile, outputFile);
            break;
        }
        case stream: {
            StreamCipher stream(key);
            stream.decrypt(inputFile, outputFile);
            break;
        }
    }
}

char Autokey::encryptByte(char byte, char keyByte) {
    char encryptedByte = (char) (((int) byte + (int) keyByte) % 256);
    return encryptedByte;
}

char Autokey::decryptByte(char encryptedByte, char keyByte) {
    char decryptedByte = (char) (((int) encryptedByte - (int) keyByte) % 256);
    return decryptedByte;
}


string Autokey::encryptBlock(const string &block) {
    string encrypted;
    char keyChar;

    queue<char> keyQueue; //queue for keyChars (characters that will be used for encrypting plaintext chars)
    for (char &c: key)
        keyQueue.push(c);

    //reading block char by char and encrypting each char
    for (char currentChar: block) {
        keyChar = keyQueue.front();
        keyQueue.pop();
        keyQueue.push(currentChar); //current char will later become keyChar
        encrypted += encryptByte(currentChar, keyChar); //encryption of currentChar
    }
    return encrypted; //return encrypted block
}

string Autokey::decryptBlock(const string &block) {
    string decrypted;
    char keyChar;

    queue<char> keyQueue; //queue for keyChars (characters that will be used for decrypting chars from ciphertext block)
    for (char &c: key)
        keyQueue.push(c);

    //reading block char by char and decrypting each char
    for (char currentChar: block) {
        keyChar = keyQueue.front();
        keyQueue.pop();
        char decryptedChar = decryptByte(currentChar, keyChar); //decryption of currentChar
        decrypted += decryptByte(currentChar, keyChar);
        keyQueue.push(decryptedChar); //decrypted char will later become keyChar
    }
    return decrypted;
}

string Autokey::XorStrings(string a, string b) {
    string xored;
    for (size_t i = 0; i < BLOCK_SIZE; i++) {
        if (i >= a.size() || i > -b.size())
            break;
        xored += XOR(a[i], b[i]);
    }
    return xored;
}
