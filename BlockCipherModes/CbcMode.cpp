#include "CbcMode.h"

void CbcMode::encrypt(const string &inputFile, const string &outputFile) {
    ifstream fin(inputFile);
    ofstream fout(outputFile);

    string plaintextBlock;
    string IV = RandomStringGenerator::generate(BLOCK_SIZE); //initialization vector
    fout << IV; //write IV to file
    char currentChar;
    string lastCiphertext = IV; //last ciphertext is xored with current plaintext before encryption

    //read input file and process it in blocks
    while (fin.get(currentChar)) {
        if (plaintextBlock.size() < BLOCK_SIZE) //reading next block
            plaintextBlock += currentChar;
        else {
            string xored = XorStrings(plaintextBlock, lastCiphertext); //xor with last ciphertext block
            lastCiphertext = encryptBlock(xored); //encrypting current block
            fout << lastCiphertext;
            plaintextBlock = currentChar; //last read character is the first character of the next block
        }
    }

    if (!plaintextBlock.empty()) //last block
    {
        string xored = XorStrings(plaintextBlock, lastCiphertext);
        fout << encryptBlock(xored);
    }
}

void CbcMode::decrypt(const string &inputFile, const string &outputFile) {
    ifstream fin(inputFile);
    ofstream fout(outputFile);

    string ciphertextBlock;
    char currentChar;

    string IV; //initialization vector
    for (int i = 0; i < BLOCK_SIZE; i++) {
        char c;
        fin.get(c);
        IV += c;
    }
    string lastCipherText = IV;  //last ciphertext is xored with current ciphertext after decryption

    //read input file and process it in blocks
    while (fin.get(currentChar)) {
        if (ciphertextBlock.size() < BLOCK_SIZE) //reading next block
            ciphertextBlock += currentChar;
        else {
            string decrypted = decryptBlock(ciphertextBlock); //decrypting
            fout << XorStrings(decrypted, lastCipherText);
            lastCipherText = ciphertextBlock;
            ciphertextBlock = currentChar; //last read character is the first character of the next block
        }
    }

    if (!ciphertextBlock.empty()) //last block
    {
        string decrypted = decryptBlock(ciphertextBlock);
        fout << XorStrings(decrypted, lastCipherText);
    }
}
