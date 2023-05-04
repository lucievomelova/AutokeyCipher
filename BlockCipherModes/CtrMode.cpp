#include "CtrMode.h"

void CtrMode::encrypt(const string &inputFile, const string &outputFile) {
    ifstream fin(inputFile);
    ofstream fout(outputFile);

    string plaintextBlock;
    string nonce = RandomStringGenerator::generate(BLOCK_SIZE / 2);
    int counter = 0; //counter starts at 0
    fout << nonce; //write nonce into output file
    char currentChar;
    string lastAfterEncryption = nonce + counterString(counter); //combine nonce and counter

    //read input file and process it in blocks
    while (fin.get(currentChar)) {
        if (plaintextBlock.size() < BLOCK_SIZE) //reading next block
            plaintextBlock += currentChar;
        else {
            lastAfterEncryption = nonce + counterString(++counter);
            lastAfterEncryption = encryptBlock(lastAfterEncryption);
            string xored = XorStrings(plaintextBlock, lastAfterEncryption);
            fout << xored;
            plaintextBlock = currentChar; //last read character is the first character of the next block
        }
    }

    if (!plaintextBlock.empty()) //last block
    {
        lastAfterEncryption = nonce + counterString(++counter);
        lastAfterEncryption = encryptBlock(lastAfterEncryption);
        string xored = XorStrings(plaintextBlock, lastAfterEncryption);
        fout << xored;
    }
}


void CtrMode::decrypt(const string &inputFile, const string &outputFile) {
    ifstream fin(inputFile);
    ofstream fout(outputFile);

    string ciphertextBlock;
    char currentChar;
    string nonce;
    int counter = 0;

    //read nonce from file
    for (int i = 0; i < BLOCK_SIZE / 2; i++) {
        char c;
        fin.get(c);
        nonce += c;
    }
    string lastAfterEncryption = nonce + counterString(counter); //combine nonce and counter

    //read input file and process it in blocks
    while (fin.get(currentChar)) {
        if (ciphertextBlock.size() < BLOCK_SIZE) //reading next block
            ciphertextBlock += currentChar;
        else {
            lastAfterEncryption = nonce + counterString(++counter);;
            lastAfterEncryption = encryptBlock(lastAfterEncryption);
            string xored = XorStrings(ciphertextBlock, lastAfterEncryption);
            fout << xored;
            ciphertextBlock = currentChar; //last read character is the first character of the next block
        }
    }

    if (!ciphertextBlock.empty()) //last block
    {
        lastAfterEncryption = nonce + counterString(++counter);;
        lastAfterEncryption = encryptBlock(lastAfterEncryption);
        string xored = XorStrings(ciphertextBlock, lastAfterEncryption);
        fout << xored;
    }
}

string CtrMode::counterString(int counter) {
    string s = to_string(counter);
    string withZeroes; //string, that will be filled with 0 from the beginning
    for (int i = 0; i < BLOCK_SIZE / 2 - (int)s.size(); i++) //fill with 0
        withZeroes += "0";
    withZeroes += s; //add counter value to the end
    return withZeroes;
}

