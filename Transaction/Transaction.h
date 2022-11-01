#pragma once

#include <iostream>
#include <vector>
#include <iomanip>
#include "../Hasher/Hasher.h"

using std::left;
using std::right;
using std::setw;
using std::string;
using std::vector;

struct Operation
{
    Operation(string userPK, int amount) : userPK(userPK), amount(amount){};
    string userPK;
    int amount;
};

static Hasher hasher;

class Transaction
{
    vector<Operation> in;
    vector<Operation> out;
    string txID;

public:
    Transaction();
    Transaction(string id);
    void setTxID(string id);
    void addInput(string senderPK, int amount);
    void addOutput(string recieverPK, int amount);
    string getTxID() const;
    vector<Operation> getInputs();
    vector<Operation> getOutputs();

    bool isTransactionHashValid();

    void printFormatedTransaction();
};