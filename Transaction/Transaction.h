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
    Transaction(string id);
    string getTxID();
    void addInput(string senderPK, int amount);
    void addOutput(string recieverPK, int amount);
    vector<Operation> getInputs();
    vector<Operation> getOutputs();
};