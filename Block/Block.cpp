#include "Block.h"

Block::Block(string prevHash, int timestamp, int version, string merkleRootHash, int nonce, int difficulty, vector<Transaction> transactions)
{
    this->prevHash = prevHash;
    this->timestamp = timestamp;
    this->version = version;
    this->merkleRootHash = merkleRootHash;
    this->nonce = nonce;
    this->difficulty = difficulty;
    this->transactions = transactions;
}