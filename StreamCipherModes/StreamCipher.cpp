#include "StreamCipher.h"

void StreamCipher::encrypt(const string &inputFile, const string &outputFile)  {
    ifstream fin(inputFile);
    ofstream fout(outputFile);

    //queue for keyChars (characters that will be used for encrypting chars from plaintext)
    queue<char>keyQueue;
    for(char & c : key)
        keyQueue.push(c);

    char currentChar, keyChar;

    //reading file char by char and encrypting each char
    while (fin.get(currentChar))
    {
        keyChar = keyQueue.front();
        keyQueue.pop();
        keyQueue.push(currentChar); //current char will later become keyChar
        char encrypted = encryptByte(currentChar, keyChar); 
        fout << encrypted;
    }
}

void StreamCipher::decrypt(const string &inputFile, const string &outputFile)  {
    ifstream fin(inputFile);
    ofstream fout(outputFile);

    //queue for keyChars (characters that will be used for decrypting chars from ciphertext)
    queue<char>keyQueue;
    for(char & c : key)
        keyQueue.push(c);

    char currentChar, keyChar;

    //reading file char by char and encrypting each char
    while (fin.get(currentChar))
    {
        keyChar = keyQueue.front();
        keyQueue.pop();
        char decrypted = decryptByte(currentChar, keyChar);
        fout << decrypted;
        keyQueue.push(decrypted); //decrypted char will later become keyChar
    }
}
