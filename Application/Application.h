#include <iostream>
#include <string>
#include "../Generator/Generator.h"
#include "../Client/Client.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;

enum AppMode
{
    GenUsers,
    GenTransactions,
    Mine,
    GetBlockCount,
    GetBlockInfo,
};

class Application
{
private:
    int argc;
    char **argv;
    string text;
    AppMode mode;
    Generator gen;
    Client cli;

    void processMode();
    void findMode();

public:
    Application(int argc, char **argv);
    void run();
};
