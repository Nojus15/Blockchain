#include "Generator.h"

string Generator::genName()
{
    int length = this->genInt(4, 10);
    string name;
    bool isVowel = this->genInt(0, 1);
    int pos;
    for (int i = 0; i < length; i++)
    {
        if (isVowel)
        {
            pos = this->genInt(0, vowelsSize);
            name.push_back(vowels[pos]);
        }
        else
        {
            pos = this->genInt(0, consonantsSize);
            name.push_back(consonants[pos]);
        }
        isVowel = !isVowel;
    }
    name[0] = toupper(name[0]);
    return name.data();
}
string Generator::genString(size_t length)
{
    string res;
    int pos = 0;

    for (size_t i = 0; i < length; i++)
    {
        pos = this->genInt(0, charSetSize);
        res.push_back(charSet[pos]);
    }
    return res;
}
int Generator::genInt(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(mt);
}
void Generator::genUsersFile(int count, string fileName)
{
    stringstream os;

    for (int i = 0; i < count; i++)
    {
        os << setw(15) << left << this->genName() << " " << hasher.hashString(this->genString(100)) << " " << this->genInt(100, 100000) << endl;
    }
    file.writeFile(fileName, os);
}
