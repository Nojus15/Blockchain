#include "Generator.h"

string Generator::genName()
{
    string name = "";
    int length = this->genInt(4, 10);
    bool isVowel = this->genInt(0, 1);
    int pos = 0;
    for (int i = 0; i < length; i++)
    {
        if (isVowel)
        {
            pos = this->genInt(0, vowelsSize);
            name += vowels[pos];
        }
        else
        {
            pos = this->genInt(0, consonantsSize);
            name += consonants[pos];
        }
        isVowel = !isVowel;
    }
    name[0] = toupper(name[0]);
    return name;
}
string Generator::genString(size_t length)
{
    string res = "";
    int pos = 0;

    for (size_t i = 0; i < length; i++)
    {
        pos = this->genInt(0, charSetSize);
        res += charSet[pos];
    }
    return res;
}
int Generator::genInt(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(mt);
}