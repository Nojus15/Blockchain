#include "Block.h"

Block::Block(string hash, string prevHash, string timestamp, string version, string merkleRootHash, int nonce, int difficulty, vector<Transaction> &transactions, int threadNr)
{
    this->hash = hash;
    this->prevHash = prevHash;
    this->timestamp = timestamp;
    this->version = version;
    this->merkleRootHash = merkleRootHash;
    this->nonce = nonce;
    this->difficulty = difficulty;
    this->transactions = transactions;
    this->threadNumber = threadNr;
}
Block::Block(string prevHash, string version, int difficulty, vector<Transaction> &transactions)
{
    this->prevHash = prevHash;
    this->version = version;
    this->difficulty = difficulty;
    this->transactions = transactions;
    this->nonce = 0;
    this->calcMerkleHash();
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
        if (nonce % 500 == 0)
            cout << "Thread: " << omp_get_thread_num() << " Nonce: " << nonce << " Target: " << target << " Currend guess: " << hash << '\r';
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
void Block::printFormatedBlockInfo()
{
    cout << endl;
    cout << string(150, '-') << endl;
    cout << endl;

    cout << setw(15) << left << "Hash:" << setw(64) << right << hash << setw(15) << left << "" << setw(20) << left << "Nonce:" << setw(20) << right << nonce << endl;
    cout << setw(15) << left << "Prev hash:" << setw(64) << right << prevHash << setw(15) << left << "" << setw(20) << left << "Transaction count:" << setw(20) << right << transactions.size() << endl;
    cout << setw(15) << left << "Merkle hash:" << setw(64) << right << merkleRootHash << setw(15) << left << "" << setw(20) << left << "Time stamp:" << setw(20) << right << timestamp << endl;
    cout << setw(15) << left << "Target hash:" << setw(64) << right << calcDifficultyTargetHash() << setw(15) << left << "" << setw(20) << left << "Difficulty:" << setw(20) << right << difficulty << endl;
    cout << setw(15) << left << "" << setw(64) << right << "" << setw(15) << left << "" << setw(20) << left << "Mined by thread nr:" << setw(20) << right << threadNumber << endl;

    cout << endl;
    cout << string(150, '-') << endl;
    cout << endl;
};
void Block::appendBlockToFile()
{
    stringstream os;
    os << hash << " " << merkleRootHash << " " << prevHash << " " << difficulty << " " << timestamp << " " << nonce << " " << version << " " << threadNumber << endl;
    for (auto &tx : transactions)
    {
        os << tx.getTxID() << endl;

        for (auto &in : tx.getInputs())
            os << in.userPK << " " << in.amount << " ";
        os << endl;
        for (auto &out : tx.getOutputs())
            os << out.userPK << " " << out.amount << " ";
        os << endl;
    }
    os << string(66, '-') << endl;

    File file;
    file.appendToFile("blocks.txt", os);
}
