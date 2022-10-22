#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

class User
{
private:
    string name;
    string publicKey;
    int balance;

public:
    User(string name, string pk, int bal);

    string getName();
    string getPublicKey();
    int getBalance();

    void setName(string name);
    void setPublicKey(string key);
    void setBalance(int bal);
};