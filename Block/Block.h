#pragma once

#include <iostream>
#include <vector>
#include "../Transaction/Transaction.h"

using std::string;
using std::vector;

class Block
{
private:
    string hash;

    // header
    string prevHash;
    string timestamp;
    string version;
    string merkleRootHash;
    int nonce;
    int difficulty;

    // body
    vector<Transaction> transactions;

public:
    Block(string hash, string prevHash, string timestamp, string version, string merkleRootHash, int nonce, int difficulty, vector<Transaction> transactions);
    string getBlockHash();
    string getPrevHash();
    string getTimestamp();
    string getVersion();
    string getMekleRootHash();
    Transaction *getTransaction(string id);
    size_t getTransactionCount();
};
