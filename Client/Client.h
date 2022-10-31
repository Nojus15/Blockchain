#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <unordered_map>
#include <algorithm>
#include "../Block/Block.h"
#include "../Hasher/Hasher.h"
#include "../Transaction/Transaction.h"
#include "../Generator/Generator.h"
#include "../File/File.h"
#include "../MerkleTree/MerkleTree.h"
#include "../Timer/Timer.h"
#include <omp.h>

using std::cout;
using std::end;
using std::getline;
using std::left;
using std::remove;
using std::right;
using std::setw;
using std::string;
using std::stringstream;
using std::unordered_map;
using std::vector;

const static int targetTime = 3;

class Client
{
    std::vector<Block> blocks;
    unordered_map<string, User> users;
    vector<Transaction> transactions;

    Generator gen;
    Hasher hasher;
    Timer blockTimer;
    Timer programTimer;

    void loadAllTransactions();
    void loadAllUsers();

    void printBlocksToFile();
    vector<Block> readBlocksFromFile();
    void printUsersToFile(unordered_map<string, User> users);

    vector<Transaction> getRandomNumberOfValidTransactions();
    void validateTransactions(vector<Transaction> &txs);
    string getMerkleRootHash(vector<Transaction> validTransactionsToBlock);
    string getTimestampAsString();
    string getPrevBlockHash();
    void updateUsersBalances(vector<Transaction> &transactionsToBlock);
    void removeAddedTransactions(vector<Transaction> &transactionsToBlock);

    void printFormatedBlockInfo(Block &block);
    vector<Block> createBlockCandidates(int count);

    int difficulty = 20;
    double allTime = 0;
    void ajustDifficulty();

public:
    void startMining(int numberOfThreads);
    void getBlockCount();
    void getBlockInfo(int pos);
};