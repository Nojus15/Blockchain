#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <omp.h>
#include "../Block/Block.h"
#include "../Transaction/Transaction.h"
#include "../Generator/Generator.h"
#include "../File/File.h"
#include "../Timer/Timer.h"

using std::cout;
using std::end;
using std::getline;
using std::remove;
using std::string;
using std::stringstream;
using std::unordered_map;
using std::vector;

// block creation target time in seconds
const static double targetTime = 1;

class Client
{
    std::vector<Block> blocks;
    unordered_map<string, User> users;
    vector<Transaction> transactions;

    Generator gen;
    Timer blockTimer;
    Timer programTimer;

    void loadAllTransactions();
    void loadAllUsers();

    void printBlocksToFile();
    vector<Block> readBlocksFromFile();
    void printUsersToFile(unordered_map<string, User> users);

    vector<Transaction> getRandomNumberOfValidTransactions();
    void validateTransactions(vector<Transaction> &txs);
    void removeAddedTransactions(vector<Transaction> &transactionsToBlock);
    void updateUsersBalances(vector<Transaction> &transactionsToBlock);

    string getPrevBlockHash();
    vector<Block> createBlockCandidates(int count);

    void ajustDifficulty();

    int difficulty = 0;
    double allTime = 0;

public:
    void startMining(int numberOfThreads);

    void getBlockCount();
    void getBlockInfo(int pos);
    void getBestHashBlock();
    void getHardestBlock();
    void getAverageDifficulty();
    void getBlockTransaction(int blockPos, int transactionPos);
};