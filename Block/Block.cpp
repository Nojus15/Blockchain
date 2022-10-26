#include "Block.h"

Block::Block(string hash, string prevHash, string timestamp, string version, string merkleRootHash, int nonce, int difficulty, vector<Transaction> transactions)
{
    this->hash = hash;
    this->prevHash = prevHash;
    this->timestamp = timestamp;
    this->version = version;
    this->merkleRootHash = merkleRootHash;
    this->nonce = nonce;
    this->difficulty = difficulty;
    this->transactions = transactions;
}
string Block::getBlockHash()
{
    return this->hash;
};
string Block::getPrevHash()
{
    return this->prevHash;
};
string Block::getTimestamp()
{
    return this->timestamp;
};
string Block::getVersion()
{
    return this->version;
};
string Block::getMekleRootHash()
{
    return this->merkleRootHash;
};
Transaction *Block::getTransaction(string id)
{
    for (auto &tx : this->transactions)
    {
        if (tx.getTxID() == id)
            return &tx;
    }
    return nullptr;
};
Transaction *Block::getTransaction(string id)
{
    for (auto &tx : this->transactions)
    {
        if (tx.getTxID() == id)
            return &tx;
    }
    return nullptr;
};
vector<Transaction> Block::getAllTransaction()
{
    return this->transactions;
}
size_t Block::getTransactionCount()
{
    return this->transactions.size();
};
