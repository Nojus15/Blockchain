#pragma once

#include <iostream>
#include <vector>
#include <chrono>

#include "../Transaction/Transaction.h"
#include "../Hasher/Hasher.h"
#include "../MerkleTree/MerkleTree.h"

#include <omp.h>

using std::string;
using std::to_string;
using std::vector;

static char symbols[] = "0123456789abcdef";

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

    int threadNumber = -1;

    void calcMerkleHash();
    void calcTimestamp();

public:
    Block(string hash, string prevHash, string timestamp, string version, string merkleRootHash, int nonce, int difficulty, vector<Transaction> &transactions);
    Block(string prevHash, string version, int difficulty, vector<Transaction> &transactions);
    string getBlockHash();
    string getPrevHash();
    string getTimestamp();
    string getVersion();
    string getMekleRootHash();
    Transaction *getTransaction(string id);
    vector<Transaction> &getAllTransactions();
    size_t getTransactionCount();
    string calcDifficultyTargetHash();
    int getNonce();
    int getDifficulty();
    int getMinedThreadNumber();

    bool mine(bool &isMined);
};
