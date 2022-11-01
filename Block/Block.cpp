#include "Block.h"

Block::Block(string hash, string prevHash, string timestamp, string version, string merkleRootHash, int nonce, int difficulty, vector<Transaction> &transactions)
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
Block::Block(string prevHash, string version, int difficulty, vector<Transaction> &transactions)
{
    cout << "Constructing block with " << transactions.size() << " transactions" << endl;
    this->prevHash = prevHash;
    this->version = version;
    this->difficulty = difficulty;
    cout << "Setting transactions" << endl;
    this->transactions = transactions;
    cout << "Transactions set" << endl;
    this->nonce = 0;
    this->calcMerkleHash();
    cout << "Merkle calculated" << endl;
    this->calcTimestamp();
};

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
vector<Transaction> &Block::getAllTransactions()
{
    return this->transactions;
}
size_t Block::getTransactionCount()
{
    return this->transactions.size();
};
int Block::getNonce()
{
    return this->nonce;
};
int Block::getDifficulty()
{
    return this->difficulty;
};
string Block::calcDifficultyTargetHash()
{
    string target(64, 'f');

    int zeroCount = difficulty / 16;
    int lastLet = 16 - difficulty % 16;

    for (int i = 0; i < zeroCount; i++)
        target[i] = '0';
    if (lastLet != 16)
        target[zeroCount] = symbols[lastLet];
    return target;
};
int Block::getMinedThreadNumber()
{
    return this->threadNumber;
};

bool Block::mine(bool &isMined)
{
    Hasher hasher;
    string target = this->calcDifficultyTargetHash();

    while (!isMined && target < (hash = hasher.hashString(prevHash + timestamp + version + to_string(nonce) + merkleRootHash + to_string(difficulty))))
    {
        nonce++;
    }

    if (!isMined && hash < target)
    {
        isMined = true;
        threadNumber = omp_get_thread_num();
        return true;
    }

    return false;
};

void Block::calcMerkleHash()
{
    MerkleTree *merkleBuilder = new MerkleTree();

    for (auto &el : transactions)
        merkleBuilder->addTransaction(el.getTxID());

    merkleBuilder->genMerkelTree();
    merkleRootHash = merkleBuilder->getRootHash();
};
void Block::calcTimestamp()
{
    timestamp = std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
};