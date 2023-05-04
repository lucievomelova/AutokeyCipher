#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

#ifndef AUTOKEYCIPHER_CLI_H
#define AUTOKEYCIPHER_CLI_H


enum Option {
    encrypt,
    decrypt
};


enum ProgramMode {
    cmdline,
    interactive
};


enum CipherMode {
    ECB,
    CBC,
    OFB,
    CTR,
    stream
};


struct Parameters
{
    string inputFile;
    string outputFile;
    string key;
    Option option;
    CipherMode mode = CipherMode::stream;

};

class CLI {
public:
    Parameters parameters;

    static void printHelp();

    ///Interactive mode - when the program is called without parameters.
    ///User is asked for one information at a time, after everything is entered correctly, the encryption/decryption
    ///of the specified file happens and the execution ends
    void interactiveMode();

    ///If user doesn't specify output filename, one is generated for him from the input filename
    string getDefaultOutputFileName() const;

    ///This function handles when the program is called from command line (if all required parameters are present)
    bool parseCmdLine(int argc, char * argv[]);

    ///Load if program will encrypt or decrypt specified file
    ///returns true if given option was valid, otherwise returns false
    bool loadOption(const string& option, ProgramMode mode);

    ///Load string *name* into parameters.outputFile. If name is an empty string,
    /// the default output file name will be used
    ///this method also checks that the file can be opened.
    ///returns true if file can be opened, otherwise it returns false
    bool loadOutputFile(const string& name);

    ///Load string *name* into parameters.inputFile
    ///this method also checks that the file can be opened.
    ///returns true if file can be opened, otherwise it returns false
    bool loadInputFile(const string& name);

    ///load *mode* into parameters.mode
    ///returns true if *mode* is a valid cipher mode, otherwise it returns false
    bool loadCipherMode(const string& mode);

    ///load *key* into parameters.key
    ///returns true if *key* is not an empty string
    bool loadKey(const string& key);

    ///Check if *str* is equal to one of the specified synonyms
    static bool isCmdlOption(const string& str, const vector<string>& synonyms)
    {
        for(const auto& synonym : synonyms) {
            if (str == synonym) {
                return true;
            }
        }
        return false;
    }
};

#endif //AUTOKEYCIPHER_CLI_H
