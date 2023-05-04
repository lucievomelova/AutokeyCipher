#include "OfbMode.h"

void OfbMode::encrypt(const string &inputFile, const string &outputFile) {
    ifstream fin(inputFile);
    ofstream fout(outputFile);

    string plaintextBlock;
    string IV = RandomStringGenerator::generate(BLOCK_SIZE); //initialization vector
    fout << IV; //write IV to file
    char currentChar;
    string lastAfterEncryption = IV; //last ciphertext is xored with current plaintext after encryption

    //read input file and process it in blocks
    while (fin.get(currentChar)) {
        if (plaintextBlock.size() < BLOCK_SIZE) //reading next block
            plaintextBlock += currentChar;
        else {
            lastAfterEncryption = encryptBlock(lastAfterEncryption); //encrypting current block
            string xored = XorStrings(plaintextBlock, lastAfterEncryption); //xor with last encrypted block
            fout << xored;
            plaintextBlock = currentChar; //last read character is the first character of the next block
        }
    }

    if (!plaintextBlock.empty()) //last block
    {
        lastAfterEncryption = encryptBlock(lastAfterEncryption);
        string xored = XorStrings(plaintextBlock, lastAfterEncryption);
        fout << xored;
    }
}

void OfbMode::decrypt(const string &inputFile, const string &outputFile) {
    ifstream fin(inputFile);
    ofstream fout(outputFile);

    string ciphertextBlock;
    char currentChar;
    string IV;

    for (int i = 0; i < BLOCK_SIZE; i++) {
        char c;
        fin.get(c);
        IV += c;
    }
    string lastAfterDecryption = IV;

    //read input file and process it in blocks
    while (fin.get(currentChar)) {
        if (ciphertextBlock.size() < BLOCK_SIZE) //reading next block
            ciphertextBlock += currentChar;
        else {
            //in ofb mode, both encryption and decryption are handled by encrypting last ciphertext
            //so even in decrypt() we call encryptBlock()
            lastAfterDecryption = encryptBlock(lastAfterDecryption);
            string xored = XorStrings(ciphertextBlock, lastAfterDecryption);
            fout << xored;
            ciphertextBlock = currentChar; //last read character is the first character of the next block
        }
    }

    if (!ciphertextBlock.empty()) //last block
    {
        lastAfterDecryption = encryptBlock(lastAfterDecryption);
        string xored = XorStrings(ciphertextBlock, lastAfterDecryption);
        fout << xored;
    }
}