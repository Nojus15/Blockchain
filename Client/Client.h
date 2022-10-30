#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <unordered_map>
#include "../Block/Block.h"
#include "../Hasher/Hasher.h"
#include "../Transaction/Transaction.h"
#include "../Generator/Generator.h"
#include "../File/File.h"
#include "../MerkleTree/MerkleTree.h"
#include "../Timer/Timer.h"

using std::cout;
using std::end;
using std::getline;
using std::left;
using std::right;
using std::setw;
using std::string;
using std::stringstream;
using std::unordered_map;
using std::vector;

static char symbols[] = "0123456789abcdef";
const static int targetTime = 3;

class Client
{
    std::vector<Block> blocks;
    Generator gen;
    Hasher hasher;
    Timer blockTimer;
    Timer programTimer;

    vector<Transaction> getAllTransactions();
    unordered_map<string, User> getAllUsers();
    string getTargetHash(int difficulty);
    void printBlocksToFile();
    vector<Block> readBlocksFromFile();
    void printUsersToFile(unordered_map<string, User> users);
    void validateTransactions(vector<Transaction> &txs);

    int difficulty = 0;
    double allTime = 0;
    void ajustDifficulty();

public:
    void startMining();
    void getBlockCount();
    void getBlockInfo(int pos);
};