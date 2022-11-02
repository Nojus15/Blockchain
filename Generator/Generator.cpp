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
void Generator::genUsersFile(int count)
{
    stringstream os;

    for (int i = 0; i < count; i++)
    {
        os << setw(15) << left << this->genName() << " " << hasher.hashString(this->genString(100)) << " " << this->genInt(100, 100000) << endl;
    }
    file.writeFile("users.txt", os);
}

void Generator::genTransactionsFile(int count)
{

    stringstream usersSS = file.readFile("users.txt");
    vector<User> users;

    string name;
    string publicKey;
    int balance;

    while (usersSS >> name && usersSS >> publicKey && usersSS >> balance)
    {
        users.emplace_back(name, publicKey, balance);
    }

    int userCount = users.size() - 1;
    Transaction *tx;
    User *sender, *reciever;
    int paymentAmount, inputedAmount;

    vector<Transaction> txs(count);

    Timer timer;
    timer.Start();

    for (int i = 0; i < count; i++)
    {
        sender = &users.at(this->genInt(0, userCount));
        reciever = &users.at(this->genInt(0, userCount));

        if (sender == reciever)
        {
            i--;
            continue;
        }
        paymentAmount = this->genInt(0, sender->getBalance() / 2);
        inputedAmount = this->genInt(paymentAmount, sender->getBalance());
        tx = &txs.at(i);
        tx->addInput(sender->getPublicKey(), inputedAmount);
        tx->addOutput(sender->getPublicKey(), inputedAmount - paymentAmount);
        tx->addOutput(reciever->getPublicKey(), paymentAmount);

        string valToHash = "";

        for (auto &in : tx->getInputs())
        {
            valToHash += in.userPK;
            valToHash += std::to_string(in.amount);
        }
        for (auto &out : tx->getOutputs())
        {
            valToHash += out.userPK;
            valToHash += std::to_string(out.amount);
        }

        tx->setTxID(this->hasher.hashString(valToHash));
    }
    cout << "Transaction generation time: " << timer.Stop() << "s" << endl;

    stringstream os;

    for (auto &tx : txs)
    {
        os << tx.getTxID() << endl;
        for (auto &in : tx.getInputs())
            os << in.userPK << " " << in.amount << " ";
        os << endl;
        for (auto &out : tx.getOutputs())
            os << out.userPK << " " << out.amount << " ";
        os << endl;
    }
    file.writeFile("transactions.txt", os);
};
