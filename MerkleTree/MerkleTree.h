#pragma once

#include <iostream>
#include <vector>
#include <map>
#include "../Hasher/Hasher.h"

using std::string;
using std::vector;

struct Node
{
    Node() : parent(nullptr), left(nullptr), right(nullptr), hash(){};
    Node(string &hash) : parent(nullptr), left(nullptr), right(nullptr), hash(hash){};

    Node *parent;
    Node *left, *right;

    string hash;
};

class MerkleTree
{
private:
    Node *root;
    std::map<string, Node *> leaves;
    Hasher hasher;

    string getSiblingHash();
    Node *findTransaction(string txId);
    Node *buildMerkelTree(vector<Node *> &line);

public:
    void addTransaction(string txId);
    // bool validateTransaction(string txId);
    void genMerkelTree();
    string getRootHash();
    Node *getRoot();
};