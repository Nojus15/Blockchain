#pragma once

#include <iostream>
#include <vector>

using std::string;
using std::vector;

struct Operation
{
    Operation(string userPK, int amount) : userPK(userPK), amount(amount){};
    string userPK;
    int amount;
};

class Transaction
{
private:
    vector<Operation> in;
    vector<Operation> out;
    string txID;

public:
    Transaction();
    Transaction(string id);
    void setTxID(string id);
    void addInput(string senderPK, int amount);
    void addOutput(string recieverPK, int amount);
    string getTxID();
    vector<Operation> getInputs();
    vector<Operation> getOutputs();
};