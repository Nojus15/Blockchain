#include <iostream>
#include <string>
#include "../Generator/Generator.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;

enum AppMode
{
    GenUsers,
};

class Application
{
private:
    int argc;
    char **argv;
    string text;
    AppMode mode;

    void processMode();
    void findMode();

public:
    Application(int argc, char **argv);
    void run();
};
