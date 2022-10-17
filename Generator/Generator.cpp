#include "Generator.h"

string Generator::genName()
{
    int length = this->genInt(4, 10);
    char *name = new char[length];
    bool isVowel = this->genInt(0, 1);
    int pos;
    for (int i = 0; i < length; i++)
    {
        if (isVowel)
        {
            pos = this->genInt(0, vowelsSize);
            name[i] = vowels[pos];
            cout << isVowel << " " << pos << " " << vowels[pos] << endl;
        }
        else
        {
            pos = this->genInt(0, consonantsSize);
            name[i] = consonants[pos];
            cout << isVowel << " " << pos << " " << consonants[pos] << endl;
        }
        isVowel = !isVowel;
    }
    name[0] = toupper(name[0]);
    cout << "returning name " << name << endl;
    return name;
}
string Generator::genString(size_t length)
{
    string res = "";
    int pos = 0;

    for (size_t i = 0; i < length; i++)
    {
        pos = this->genInt(0, charSetSize);
        res[i] = charSet[pos];
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
        // os << this->genName() << " " << this->genString(64) << " " << this->genInt(100, 1000000) << endl;
        os << "TEST " << this->genName() << endl;
    }
    file.writeFile(fileName, os);
}
