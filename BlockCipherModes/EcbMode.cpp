#include "EcbMode.h"

void EcbMode::encrypt(const string &inputFile, const string &outputFile) {
    ifstream fin(inputFile);
    ofstream fout(outputFile);
    string plaintextBlock;
    char currentChar;

    //read input file and process it in blocks
    while (fin.get(currentChar)) {
        if (plaintextBlock.size() < BLOCK_SIZE) //reading next block
            plaintextBlock += currentChar;
        else {
            fout << encryptBlock(plaintextBlock);
            plaintextBlock = currentChar; //last read character is the first character of the next block
        }
    }

    if (!plaintextBlock.empty()) //last block
        fout << encryptBlock(plaintextBlock);
}

void EcbMode::decrypt(const string &inputFile, const string &outputFile) {
    ifstream fin(inputFile);
    ofstream fout(outputFile);
    string ciphertextBlock;
    char currentChar;

    //read input file and process it in blocks
    while (fin.get(currentChar)) {
        if (ciphertextBlock.size() < BLOCK_SIZE) //reading next block
            ciphertextBlock += currentChar;
        else {
            fout << decryptBlock(ciphertextBlock);
            ciphertextBlock = currentChar; //last read character is the first character of the next block
        }
    }

    if (!ciphertextBlock.empty()) //last block
        fout << decryptBlock(ciphertextBlock);
}
