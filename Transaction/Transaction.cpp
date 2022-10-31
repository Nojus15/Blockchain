#include "Transaction.h"

Transaction::Transaction(){};
Transaction::Transaction(string id)
{
    this->txID = id;
}
void Transaction::setTxID(string id)
{
    this->txID = id;
};

void Transaction::addInput(string senderPK, int amount)
{
    this->in.emplace_back(senderPK, amount);
};
void Transaction::addOutput(string recieverPK, int amount)
{
    this->out.emplace_back(recieverPK, amount);
};
string Transaction::getTxID() const
{
    return this->txID;
};
vector<Operation> Transaction::getInputs()
{
    return this->in;
};
vector<Operation> Transaction::getOutputs()
{
    return this->out;
};
bool Transaction::isTransactionHashValid()
{
    string valToHash = "";

    for (auto &in : this->in)
    {
        valToHash += in.userPK;
        valToHash += std::to_string(in.amount);
    }
    for (auto &out : this->out)
    {
        valToHash += out.userPK;
        valToHash += std::to_string(out.amount);
    }

    return txID == hasher.hashString(valToHash);
};
