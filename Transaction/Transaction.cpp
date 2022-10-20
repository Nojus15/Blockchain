#include "Transaction.h"

Transaction::Transaction(string id)
{
    this->txID = id;
}

string Transaction::getTxID()
{
    return this->txID;
};
void Transaction::addInput(string senderPK, int amount)
{
    this->in.emplace_back(senderPK, amount);
};
void Transaction::addOutput(string recieverPK, int amount)
{
    this->out.emplace_back(recieverPK, amount);
};
vector<Operation> Transaction::getInputs()
{
    return this->in;
};
vector<Operation> Transaction::getOutputs()
{
    return this->out;
};