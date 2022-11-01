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
void Transaction::printFormatedTransaction()
{
    cout << endl;
    cout << string(200, '*') << endl;
    cout << endl;

    cout << "Transaction id: " << txID << endl;
    cout << endl;
    cout << setw(87) << left << "Inputs:" << left << "  |  " << left << "Outputs:" << endl;
    cout << setw(87) << left << "" << left << "  |  " << endl;
    for (int i = 0; i < in.size() || i < out.size(); i++)
    {
        if (i < in.size())
            cout << setw(15) << left << "Public key:" << setw(64) << in.at(i).userPK << setw(8) << right << in.at(i).amount << setw(5) << "  |  ";
        else
            cout << setw(15) << "" << setw(64) << "" << setw(8) << right << "" << setw(5) << "  |  ";

        if (i < out.size())
            cout << setw(15) << left << "Public key:" << setw(64) << out.at(i).userPK << setw(8) << right << out.at(i).amount << endl;
        else
            cout << endl;
    }

    cout << endl;
    cout << string(200, '*') << endl;
    cout << endl;
};
