#include "User.h"

User::User(string name, string pk, int bal)
{
    this->name = name;
    this->publicKey = pk;
    this->balance = bal;
};

string User::getName()
{
    return this->name;
}

void User::setName(string name)
{
    this->name = name;
}

string User::getPublicKey()
{
    return this->publicKey;
}
void User::setPublicKey(string publicKey)
{
    this->publicKey = publicKey;
}

int User::getBalance()
{
    return this->balance;
}

void User::setBalance(int balance)
{
    this->balance = balance;
}
void User::updateBalance(int balance)
{
    this->balance += balance;
    imaginaryBalance = -1;
}
