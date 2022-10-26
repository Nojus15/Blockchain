#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include "../Block/Block.h"
#include "../Hasher/Hasher.h"
#include "../Transaction/Transaction.h"
#include "../Generator/Generator.h"
#include "../File/File.h"
#include "../MerkleTree/MerkleTree.h"
#include <signal.h>

using std::cout;
using std::end;
using std::getline;
using std::left;
using std::right;
using std::setw;
using std::string;
using std::stringstream;
using std::vector;

class Client
{
    std::vector<Block> blocks;
    Generator gen;
    Hasher hasher;

    vector<Transaction> getAllTransactions();
    string getTargetHash(int difficulty);
    void printBlocksToFile();

public:
    void startMining();
};