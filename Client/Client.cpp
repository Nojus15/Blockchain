#include "Client.h"

void Client::startMining()
{
    vector<Transaction> allTransactions(this->getAllTransactions());
    unordered_map<string, User> users(this->getAllUsers());
    string target, merkleHash, timestamp, prevHash, guess;
    int nonce = 0;
    string version = "1";

    int count, pos;
    programTimer.Start();
    while (allTransactions.size() > 0)
    {
        count = gen.genInt(1, allTransactions.size() > 100 ? 100 : allTransactions.size());
        pos = gen.genInt(0, allTransactions.size() - count);

        vector<Transaction> txsToBlock(allTransactions.begin() + pos, allTransactions.begin() + pos + count);
        this->validateTransactions(txsToBlock);

        MerkleTree *merkleBuilder = new MerkleTree();
        for (auto &el : txsToBlock)
            merkleBuilder->addTransaction(el.getTxID());
        merkleBuilder->genMerkelTree();

        this->ajustDifficulty();
        target = this->getTargetHash(difficulty);
        timestamp = std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
        merkleHash = merkleBuilder->getRootHash();
        prevHash = this->blocks.empty() ? "0000000000000000000000000000000000000000000000000000000000000000" : this->blocks.back().getBlockHash();

        blockTimer.Start();
        while (target < (guess = this->hasher.hashString(prevHash + timestamp + version + to_string(nonce) + merkleHash + to_string(difficulty))))
        {
            if (nonce % 1000 == 0)
            {
                cout << "Nonce: " << nonce << " Guess: " << guess << '\r';
                timestamp = std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
            }
            nonce++;
        }
        allTime += blockTimer.Stop();
        cout << std::string(100, ' ') << endl; // to remove last line

        this->blocks.emplace_back(guess, prevHash, timestamp, version, merkleHash, nonce, difficulty, txsToBlock);

        cout << setw(15) << left << "Target:" << setw(64) << right << target << setw(15) << left << "" << setw(18) << left << "Difficulty: " << setw(20) << right << difficulty << endl;
        cout << setw(15) << left << "Guess:" << setw(64) << right << guess << setw(15) << left << "" << setw(18) << left << "Nonce:" << setw(20) << right << nonce << endl;
        cout << setw(15) << left << "Prev hash:" << setw(64) << right << prevHash << setw(15) << left << "" << setw(18) << left << "Transaction count:" << setw(20) << right << txsToBlock.size() << endl;
        cout << setw(15) << left << "Merkle hash:" << setw(64) << right << merkleHash << setw(15) << left << "" << setw(18) << left << "Time stamp:" << setw(20) << right << timestamp << endl;
        cout << endl;
        cout << "-----------------------------------------------------" << endl;

        nonce = 0;
        allTransactions.erase(allTransactions.begin() + pos, allTransactions.begin() + pos + count);
        for (auto &tx : txsToBlock)
        {
            for (auto &in : tx.getInputs())
                users.at(in.userPK).updateBalance(in.amount);
            for (auto &out : tx.getOutputs())
                users.at(out.userPK).updateBalance(out.amount * -1);
        }
    }
    cout << "Average block mine time: " << programTimer.Stop() / this->blocks.size() << endl;
    this->printBlocksToFile();
    this->printUsersToFile(users);
}
vector<Transaction> Client::getAllTransactions()
{
    File file;
    stringstream txSS = file.readFile("../txt_files/transactions.txt");

    vector<Transaction> transactions;

    string tempIn, tempOut, txID;
    std::stringstream inputs, outputs;
    string userPK;
    int amount;

    while (txSS)
    {
        getline(txSS, txID);
        getline(txSS, tempIn);
        getline(txSS, tempOut);

        if (txID.length() == 0 || tempOut.length() == 0)
            break;

        inputs << tempIn;
        outputs << tempOut;

        Transaction *t = new Transaction(txID);
        while (inputs >> userPK && inputs >> amount)
            t->addInput(userPK, amount);
        while (outputs >> userPK && outputs >> amount)
            t->addOutput(userPK, amount);
        transactions.push_back(*t);
        inputs.clear();
        outputs.clear();
    }
    return transactions;
}
unordered_map<string, User> Client::getAllUsers()
{
    std::unordered_map<string, User> users;
    File file;
    stringstream usersSS = file.readFile("users.txt");

    string name;
    string publicKey;
    int balance;

    while (usersSS >> name && usersSS >> publicKey && usersSS >> balance)
    {
        users.insert(std::make_pair(publicKey, User(name, publicKey, balance)));
    }
    return users;
};

string Client::getTargetHash(int difficulty)
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
void Client::printBlocksToFile()
{
    stringstream os;
    for (auto &block : this->blocks)
    {
        os << block.getBlockHash() << " " << block.getMekleRootHash() << " " << block.getPrevHash() << " " << block.getDifficulty() << " " << block.getTimestamp() << " " << block.getNonce() << " " << block.getVersion() << " " << block.getTransactionCount() << endl;
        for (auto &tx : block.getAllTransactions())
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
    }

    File file;
    file.writeFile("blocks.txt", os);
};
vector<Block> Client::readBlocksFromFile()
{
    File file;
    stringstream blocksSS = file.readFile("blocks.txt");
    vector<Block> blocks;

    string temp, blockInfo;
    std::stringstream inputs, outputs;
    string userPK;
    int amount;

    while (blocksSS)
    {
        Transaction *t;
        getline(blocksSS, blockInfo);

        if (blockInfo.length() == 0)
            break;

        stringstream line(blockInfo);

        string hash, prevHash, timestamp, version, merkleRootHash;
        int nonce, difficulty;
        vector<Transaction> transactions;

        line >> hash >> merkleRootHash >> prevHash >> difficulty >> timestamp >> nonce >> version;
        while (blocksSS)
        {
            getline(blocksSS, temp);
            if (temp == string(66, '-'))
                break;

            t = new Transaction(temp);
            getline(blocksSS, temp);
            inputs << temp;
            getline(blocksSS, temp);
            outputs << temp;

            while (inputs >> userPK && inputs >> amount)
                t->addInput(userPK, amount);
            while (outputs >> userPK && outputs >> amount)
                t->addOutput(userPK, amount);
            transactions.push_back(*t);
        }

        inputs.clear();
        outputs.clear();
        blocks.emplace_back(hash, prevHash, timestamp, version, merkleRootHash, nonce, difficulty, transactions);
    }
    return blocks;
};

void Client::getBlockCount()
{
    cout << this->readBlocksFromFile().size() << endl;
};
void Client::getBlockInfo(int pos)
{
    Block block = this->readBlocksFromFile().at(pos);

    cout << setw(15) << left << "Hash:" << setw(64) << right << block.getBlockHash() << setw(15) << left << "" << setw(18) << left << "Nonce:" << setw(20) << right << block.getNonce() << endl;
    cout << setw(15) << left << "Prev hash:" << setw(64) << right << block.getPrevHash() << setw(15) << left << "" << setw(18) << left << "Transaction count:" << setw(20) << right << block.getTransactionCount() << endl;
    cout << setw(15) << left << "Merkle hash:" << setw(64) << right << block.getMekleRootHash() << setw(15) << left << "" << setw(18) << left << "Time stamp:" << setw(20) << right << block.getTimestamp() << endl;
    cout << setw(15) << left << "" << setw(64) << right << "" << setw(15) << left << "" << setw(18) << left << "Difficulty:" << setw(20) << right << block.getDifficulty() << endl;
    cout << endl;
};
void Client::printUsersToFile(unordered_map<string, User> users)
{

    stringstream os;
    File file;

    for (auto &user : users)
    {
        os << setw(15) << left << user.second.getName() << " " << user.second.getPublicKey() << " " << user.second.getBalance() << endl;
    }
    file.writeFile("users.txt", os);
}
void Client::validateTransactions(vector<Transaction> &txs)
{
    for (auto it = txs.begin(); it != txs.end(); it++)
    {
        if (!(*it).isTransactionValid())
        {
            txs.erase(it);
            it--;
        }
    }
};
void Client::ajustDifficulty()
{
    difficulty += targetTime - allTime / this->blocks.size();
    if (difficulty < 0)
        difficulty = 0;
}
