#include "CLI.h"
#include <string>


bool CLI::parseCmdLine(int argc, char **argv) {
    //if only two parameters are present, the user might be asking for help text
    if (argc == 2) {
        if (isCmdlOption(argv[1], {"-h", "--help"})) {
            printHelp();
            return false;
        }
    }

    //if only three parameters are present, the user might be asking for help text for specific mode
    else if (argc == 3) {
        string str2 = (string) argv[2];
        size_t pos = str2.find('=');
        if ((isCmdlOption(argv[1], {"-h", "--help"})) &&
            (isCmdlOption(str2.substr(0, pos), {"-m", "--mode"}))) {
            ifstream fin;
            string mode = str2.substr(pos + 1);
            if (mode == "ECB" || mode == "ecb")
                fin.open("helpTexts/ECB_chart");
            else if (mode == "CBC" || mode == "cbc")
                fin.open("helpTexts/CBC_chart");
            else if (mode == "OFB" || mode == "ofb")
                fin.open("helpTexts/OFB_chart");
            else if (mode == "CTR" || mode == "ctr")
                fin.open("helpTexts/CTR_chart");
            else if (mode == "stream")
                fin.open("helpTexts/stream_chart");

            std::cout << fin.rdbuf();
            return false;
        }
    }

    bool keyGiven = false;
    bool inputFileGiven = false;
    bool optionGiven = false;
    bool modeGiven = false;
    bool outputFileGiven = false;
    string outputFileName;

    //there are more parameters on the cmdline - try to call autokey if all needed options are present
    for (int i = 1; i < argc; i++) {
        string currentOption = (string) argv[i];
        size_t pos = currentOption.find('='); //position of '=' in option


        //encryption or decryption
        if (isCmdlOption(currentOption, {"-e", "--encrypt"})) {
            if (optionGiven) {
                cout << "You can't use flags for encryption and decryption together" << endl;
                return false;
            }
            optionGiven = true;
            parameters.option = Option::encrypt;
        } else if (isCmdlOption(currentOption, {"-d", "--decrypt"})) {
            if (optionGiven) {
                cout << "You have to use exactly 1 flag out of these four: -e, --encrypt, -d, --decrypt" << endl;
                return false;
            }
            optionGiven = true;
            parameters.option = Option::decrypt;
        }

        //check if option has specified parameter (because all options except for -e, -d have char '=' in them
        //so if this character is not present, given option is invalid (either it's an unknown option
        //or it's missing the parameter after '='
        else if(pos == currentOption.size()) {
            cout << "Invalid option: " << currentOption << endl;
            return false;
        }

        //loading key
        else if (isCmdlOption(currentOption.substr(0, pos), {"-k", "--key"})) {
            if (keyGiven) {
                cout << "You can't specify key more than once" << endl;
                return false;
            }
            keyGiven = true;
            if(!loadKey(currentOption.substr(pos + 1)))
                return false;
        }

        //cipher mode
        else if (isCmdlOption(currentOption.substr(0, pos), {"-m", "--mode"})) {
            if (modeGiven) {
                cout << "You can't specify cipher mode more than once" << endl;
                return false;
            }
            modeGiven = true;
            if (!loadCipherMode(currentOption.substr(pos + 1))) {
                return false;
            }
        }

        //loading input file + checking that it can be opened
        else if (isCmdlOption(currentOption.substr(0, pos), {"-i", "--input"})) {
            if (inputFileGiven) {
                cout << "You can't specify input file more than once" << endl;
                return false;
            }
            inputFileGiven = true;
            if (!loadInputFile(currentOption.substr(pos + 1)))
                return false;
        }

        //loading output file + checking that it can be opened
        else if (isCmdlOption(currentOption.substr(0, pos), {"-o", "--output"})) {
            if (outputFileGiven) {
                cout << "You can't specify output file more than once" << endl;
                return false;
            }
            outputFileGiven = true;
            outputFileName = currentOption.substr(pos + 1);
        }

        //invalid option present on cmdline
        else {
            cout << "Invalid input." << endl;
            return false;
        }
    }

    //loading output file name and checking that it can be opened
    //because it's an optional parameter, *outputFileName* can be empty. Then the default output file name will be used
    if (!loadOutputFile(outputFileName))
        return false;

    if (keyGiven && optionGiven && inputFileGiven)
        return true;
    else if (!optionGiven)
        cout << "You need to specify if you want to encrypt or decrypt the file (flags -e/--encrypt, -d/--decrypt)"
             << endl;
    else if (!keyGiven)
        cout << "You need to specify cipher key (option -k/--key='your_key')" << endl;
    else if (!inputFileGiven)
        cout << "You need to specify the input file (option -i/--input='your_file')" << endl;
    return false;
}

bool CLI::loadOutputFile(const string &name) {
    if (!name.empty())
        parameters.outputFile = name;
    else
        parameters.outputFile = getDefaultOutputFileName(); //*name* is empty, set default file name

    ofstream out(parameters.outputFile);
    if (!out) {
        cout << "File " << name <<" cannot be opened." << endl;
        return false; //file cannot be opened
    }
    return true;
}

bool CLI::loadOption(const string &option, ProgramMode mode) {
    if (mode == ProgramMode::cmdline) {
        if (option == "-e" || option == "--encrypt")
            parameters.option = Option::encrypt;
        else if (option == "-d" || option == "--decrypt")
            parameters.option = Option::decrypt;
        else
            return false;
    }
    else if (mode == ProgramMode::interactive) {
        if (option == "e" || option == "encrypt")
            parameters.option = Option::encrypt;
        else if (option == "d" || option == "decrypt")
            parameters.option = Option::decrypt;
        else
            return false;
    }

    return true;
}

bool CLI::loadInputFile(const string &name) {
    parameters.inputFile = name; //load input file name
    ifstream in(parameters.inputFile); //checking if file can be opened
    if (!in) {
        cout << "File " << name <<" does not exist or cannot be opened." << endl;
        return false; //file cannot be opened
    }
    return true;
}

void CLI::printHelp() {
    ifstream fin("helpTexts/help"); //name of file with help text
    string line;
    while (getline(fin, line)) //print file contents
        cout << line << endl;
}

void CLI::interactiveMode() {
    cout << "AutokeyCihper - program for fle encryption and decryption" << endl;
    string input;

    //interactive mode asks user for one information at a time. Until valid input is entered, the user is stuck
    //in a loop for this parameter, that's why each information is handled in a while loop

    //encrypt or decrypt
    while (true) {
        cout << "Do you want to encrypt or decrypt a file? Type 'e' or 'encrypt' for encryption ";
        cout << "and 'd' or 'decrypt' for decryption: " << endl;
        cin >> input;
        if (loadOption(input, ProgramMode::interactive)) {
            cout << "You have selected: " << (parameters.option == Option::encrypt ? "encryption" : "decryption")
                 << endl;
            break;
        } else
            cout << "Wrong option." << endl;
    }

    //input file
    while (true) {
        cout << "Enter input file name: ";
        cin >> input;
        if (loadInputFile(input))  {
            cout << "You have selected file: " << input << endl;
            break;
        } else
            cout << "Wrong input. File doesn't exist or it cannot be opened." << endl;
    }

    //output file
    while (true) {
        cout << "Enter output file name (optional - just click [ENTER] if you do not wish to specify ";
        cout << " output file name, generated output will then be stored in file ";
        cout << getDefaultOutputFileName() << "): ";

        cin.get(); //removing newline from cin
        getline(cin, input); //reading input even when it is empty (because outfile doesn't need to be specified)
        if (loadOutputFile(input)) {
            cout << "You have selected file: " << parameters.outputFile << endl;
            break;
        } else
            cout << "Something went wrong." << endl;
    }

    //key
    while (true) {
        cout << "Enter key for " << (parameters.option == Option::encrypt ? "encryption" : "decryption") << endl;
        cin >> input;
        if (loadKey(input)) {
            cout << "Key: " << parameters.key << endl;
            break;
        }
    }

    cin.get(); //removing newline from cin

    //cipher mode
    while (true) {
        cout << "Select cipher mode." << endl;
        cout << "For block cipher encryption select one of the following modes: 'ECB', 'CBC', 'OFB' or 'CTR'." << endl;
        cout << "For stream cipher mode: type 'stream'. This is also the default mode." << endl;

        getline(cin, input); //reading input even when it is empty (because mode doesn't need to be specified)
        if (input.empty())
            input = "stream";
        if (loadCipherMode(input)) {
            cout << "You have selected mode: " << input << endl;
            break;
        }
    }

    //final message (if everything was successfull)
    cout << "The file " << parameters.inputFile << " was "
         << (parameters.option == Option::encrypt ? "encrypted" : "decrypted")
         << " successfully into " << parameters.outputFile << endl;
}

string CLI::getDefaultOutputFileName() const {
    size_t extensionPos = parameters.inputFile.find_last_of('.'); //position of the last '.' character
    string beforeExtension = parameters.inputFile.substr(0, extensionPos); //filename without extension
    string extension;
    if (extensionPos < parameters.inputFile.size())
        extension = parameters.inputFile.substr(extensionPos);

    if (parameters.option == Option::encrypt)
        return beforeExtension + "_encrypted" + extension;
    else
        return beforeExtension + "_decrypted" + extension;
}

bool CLI::loadCipherMode(const string &mode) {
    if (mode == "ECB" || mode == "ecb") {
        parameters.mode = CipherMode::ECB;
        return true;
    } else if (mode == "CBC" || mode == "cbc") {
        parameters.mode = CipherMode::CBC;
        return true;
    } else if (mode == "OFB" || mode == "ofb") {
        parameters.mode = CipherMode::OFB;
        return true;
    } else if (mode == "CTR" || mode == "ctr") {
        parameters.mode = CipherMode::CTR;
        return true;
    } else if (mode == "STREAM" || mode == "stream") {
        parameters.mode = CipherMode::stream;
        return true;
    }
    cout << "Invalid cipher mode. Please select one of the following: ECB, CBC, OFB, CTR, stream." << endl;
    return false;
}

bool CLI::loadKey(const string &key) {
    if(key.empty())
    {
        cout << "Key cannot be an empty string. PLease enter a valid key. " << endl;
        return false;
    }
    parameters.key = key;
    return true;

}
