#include <iostream>
#include "Application/Application.h"

int main(int argc, char **argv)
{
    Application *app = new Application(argc, argv);
    app->run();
    return 0;
}