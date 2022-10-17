#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <random>
#include <string>
#include <cstring>
#include "../File/File.h"

using std::cin;
using std::cout;
using std::endl;
using std::mt19937;
using std::string;
using std::stringstream;
using std::to_string;

static const char charSet[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";
static const int charSetSize = sizeof(charSet) - 2;

static const char vowels[] = "aeiouy";
static const int vowelsSize = sizeof(vowels) - 2;

static const char consonants[] = "qwrtpsdfghjklzxcvbnm";
static const int consonantsSize = sizeof(consonants) - 2;

static std::random_device rd;
static std::mt19937 mt(rd());
static std::uniform_int_distribution<int> dist(0, strlen(charSet) - 1);

class Generator
{
    File file;

    string genName();
    string genString(size_t length);
    int genInt(int min, int max);

public:
    void genUsersFile(int count, string fileName);
};