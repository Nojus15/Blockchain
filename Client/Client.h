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

class Client
{
    std::vector<Block> blocks;
    Generator gen;
    Hasher hasher;

    vector<Transaction> getAllTransactions();
    unordered_map<string, User> getAllUsers();
    string getTargetHash(int difficulty);
    void printBlocksToFile();
    vector<Block> readBlocksFromFile();
    void printUsersToFile(unordered_map<string, User> users);
    void validateTransactions(vector<Transaction> &txs);

public:
    void startMining();
    void getBlockCount();
    void getBlockInfo(int pos);
};