#pragma once

#include <iostream>
#include <vector>
#include "../Transaction/Transaction.h"

using std::string;
using std::vector;

class Block
{
private:
    // header
    string prevHash;
    int timestamp;
    int version;
    string merkleRootHash;
    int nonce;
    int difficulty;

    // body
    vector<Transaction> transactions;

public:
    Block(string prevHash, int timestamp, int version, string merkleRootHash, int nonce, int difficulty, vector<Transaction> transactions);
};
