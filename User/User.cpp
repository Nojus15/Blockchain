#include "User.h"

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