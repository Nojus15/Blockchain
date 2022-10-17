#include "Application.h"

Application::Application(int argc, char **argv)
{
    if (argc > 1)
    {
        this->argc = argc;
        this->argv = argv;
    }
}
void Application::run()
{
    try
    {
        findMode();
        processMode();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}
void Application::processMode()
{
}

void Application::findMode()
{
}
