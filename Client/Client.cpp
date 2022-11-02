#include "Client.h"

void Client::startMining(int numberOfThreads)
{
    this->loadAllTransactions();
    this->loadAllUsers();
    File file;
    file.clearFile("blocks.txt");

    programTimer.Start();
    while (transactions.size() > 0)
    {
        vector<Block> blocksCandidates(this->createBlockCandidates(numberOfThreads));

        bool mined = false;
        blockTimer.Start();

        omp_set_dynamic(0);
        omp_set_num_threads(numberOfThreads);
#pragma omp parallel for
        for (int i = 0; i < numberOfThreads; i++)
        {
            bool first = blocksCandidates.at(i).mine(mined);

            if (first)
                prevBlock = blocksCandidates.at(i);
        }

        cout << string(200, ' ') << endl; // to override last line

        allTime += blockTimer.Stop();

        prevBlock.appendBlockToFile();
        prevBlock.printFormatedBlockInfo();
        this->removeAddedTransactions(prevBlock.getAllTransactions());
        this->updateUsersBalances(prevBlock.getAllTransactions());
    }
    double programTime = programTimer.Stop();
    cout << "Average block mine time: " << programTime / blockCounter << endl;
    cout << "All program time: " << programTime << endl;
    cout << "Invalid transaction count: " << invalidTransactionCounter << endl;
    this->printUsersToFile(users);
}
void Client::loadAllTransactions()
{
    File file;
    stringstream txSS = file.readFile("../txt_files/transactions.txt");

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
}
void Client::loadAllUsers()
{
    File file;
    stringstream usersSS = file.readFile("users.txt");

    string name;
    string publicKey;
    int balance;

    while (usersSS >> name && usersSS >> publicKey && usersSS >> balance)
    {
        users.insert(std::make_pair(publicKey, User(name, publicKey, balance)));
    }
};
vector<Block> Client::readBlocksFromFile()
{
    File file;
    stringstream blocksSS = file.readFile("blocks.txt");
    vector<Block> blocks;

    string temp, blockInfo;
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
        int nonce, difficulty, threadNr;
        vector<Transaction> transactions;

        line >> hash >> merkleRootHash >> prevHash >> difficulty >> timestamp >> nonce >> version >> threadNr;
        while (blocksSS)
        {
            getline(blocksSS, temp);
            if (temp == string(66, '-'))
                break;

            t = new Transaction(temp);

            getline(blocksSS, temp);
            stringstream inputs(temp);

            getline(blocksSS, temp);
            stringstream outputs(temp);

            while (inputs >> userPK && inputs >> amount)
                t->addInput(userPK, amount);
            while (outputs >> userPK && outputs >> amount)
                t->addOutput(userPK, amount);

            transactions.push_back(*t);
        }

        blocks.emplace_back(hash, prevHash, timestamp, version, merkleRootHash, nonce, difficulty, transactions, threadNr);
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
    block.printFormatedBlockInfo();
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
        bool userCanMakeTransaction = true;
        for (auto &input : (*it).getInputs())
        {
            User *user = &users.at(input.userPK);

            if (user->imaginaryBalance == -1)
                user->imaginaryBalance = user->getBalance();

            if (user->imaginaryBalance < input.amount)
                userCanMakeTransaction = false;
            else
                user->imaginaryBalance -= input.amount;
        }

        if (!(*it).isTransactionHashValid() || !userCanMakeTransaction)
        {
            string txId = (*it).getTxID();
            transactions.erase(std::find_if(transactions.begin(), transactions.end(), [txId](const Transaction &tx)
                                            { return tx.getTxID() == txId; }));
            txs.erase(it);
            it--;
            invalidTransactionCounter++;
        }
    }
};
void Client::ajustDifficulty()
{
    if (blockCounter > 0)
        difficulty += targetTime - floor(allTime / blockCounter);
    if (difficulty < 0)
        difficulty = 0;
}
vector<Transaction> Client::getRandomNumberOfValidTransactions()
{
    int count = transactions.size() >= 100 ? gen.genInt(1, 100) : transactions.size();
    int pos = gen.genInt(0, transactions.size() - count);

    vector<Transaction> transactionsToBlock(transactions.begin() + pos, transactions.begin() + pos + count);
    this->validateTransactions(transactionsToBlock);

    return transactionsToBlock;
};
string Client::getPrevBlockHash()
{
    return blockCounter == 0 ? string(64, '0') : prevBlock.getBlockHash();
}
void Client::updateUsersBalances(vector<Transaction> &transactionsToBlock)
{
    for (auto &tx : transactionsToBlock)
    {
        for (auto &in : tx.getInputs())
            users.at(in.userPK).updateBalance(in.amount * -1);
        for (auto &out : tx.getOutputs())
            users.at(out.userPK).updateBalance(out.amount);
    }
}
void Client::removeAddedTransactions(vector<Transaction> &transactionsToBlock)
{
    for (auto const &transaction : transactionsToBlock)
    {
        string txIdToDelete = transaction.getTxID();
        transactions.erase(std::remove_if(transactions.begin(), transactions.end(), [txIdToDelete](const Transaction &tx)
                                          { return tx.getTxID() == txIdToDelete; }),
                           transactions.end());
    }
};
vector<Block> Client::createBlockCandidates(int count)
{
    string version = "1";
    string prevHash = this->getPrevBlockHash();

    vector<Block> candidates;
    this->ajustDifficulty();

    for (int i = 0; i < count; i++)
    {
        vector<Transaction> transactionsToBlock(getRandomNumberOfValidTransactions());
        candidates.emplace_back(prevHash, version, this->difficulty, transactionsToBlock);
    }

    return candidates;
};

void Client::getBestHashBlock()
{
    vector<Block> blocks(this->readBlocksFromFile());

    string minHash(blocks.at(0).getBlockHash()), blockHash;
    Block *minHashBlock;

    for (auto it = blocks.begin(); it != blocks.end(); it++)
    {
        blockHash = (*it).getBlockHash();
        if (blockHash < minHash)
        {
            minHash = blockHash;
            minHashBlock = &(*it);
        }
    }
    minHashBlock->printFormatedBlockInfo();
};
void Client::getHardestBlock()
{
    vector<Block> blocks(this->readBlocksFromFile());

    int nonce, maxNonce = -1;
    Block *maxNonceBlock;

    for (auto it = blocks.begin(); it != blocks.end(); it++)
    {
        nonce = (*it).getNonce();
        if (nonce > maxNonce)
        {
            maxNonce = nonce;
            maxNonceBlock = &(*it);
        }
    }
    maxNonceBlock->printFormatedBlockInfo();
};
void Client::getAverageDifficulty()
{
    vector<Block> blocks(this->readBlocksFromFile());

    int avgDifficulty = 0;

    for (auto &block : blocks)
    {
        avgDifficulty += block.getDifficulty();
    }
    cout << "Average difficulty: " << avgDifficulty / blocks.size() << endl;
};
void Client::getBlockTransaction(int blockPos, int transactionPos)
{
    Block block(this->readBlocksFromFile().at(blockPos));
    Transaction transaction(block.getAllTransactions().at(transactionPos));

    transaction.printFormatedTransaction();
};