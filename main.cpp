#include "Autokey.h"
#include "CLI.h"

using namespace std;

int main(int argc, char * argv[])
{
    CLI cli;
    if(argc == 1)
    {
        cli.interactiveMode();
        Autokey autokey(cli.parameters.key);
        if(cli.parameters.option == Option::encrypt) {
            autokey.encryptFile(cli.parameters.inputFile, cli.parameters.outputFile, cli.parameters.mode);
        }
        else {
            autokey.decryptFile(cli.parameters.inputFile, cli.parameters.outputFile, cli.parameters.mode);
        }
    }
    else if(cli.parseCmdLine(argc, argv))
    {
        Autokey autokey(cli.parameters.key);
        if(cli.parameters.option == Option::encrypt) {
            autokey.encryptFile(cli.parameters.inputFile, cli.parameters.outputFile, cli.parameters.mode);
        }
        else {
            autokey.decryptFile(cli.parameters.inputFile, cli.parameters.outputFile, cli.parameters.mode);
        }
    }
    else
        cout << "Type autokey -h/--help for more detailed description of how to use this program." << endl;

    return 0;
}
