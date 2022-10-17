#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

enum AppMode
{
    GenFile,
    HashFile,
    HashLine,
    TestCollisions,
    TestSpeed,
    TestCollisionsByPairs,
    TestKonstitucija,
    TestAvalanche,
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
