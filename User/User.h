#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

class User
{
private:
    std::string name;
    std::string publicKey;
    int balance;

public:
    string getName();
    string getPublicKey();
    int getBalance();

    void setName(string name);
    void setPublicKey(string key);
    void setBalance(int bal);
};