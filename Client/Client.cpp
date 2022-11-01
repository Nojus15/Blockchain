#include "Client.h"

void Client::startMining(int numberOfThreads)
{
    try
    {
        this->loadAllTransactions();
        this->loadAllUsers();
        cout << users.size() << endl;

        programTimer.Start();
        while (transactions.size() > 0)
        {
            // cout << "Generating blocks candidates" << endl;
            vector<Block> blocksCandidates(this->createBlockCandidates(numberOfThreads));
            // cout << "Blocks candidates generated" << endl;

            bool mined = false;
            blockTimer.Start();

            omp_set_dynamic(0);
            omp_set_num_threads(numberOfThreads);
#pragma omp parallel for
            for (int i = 0; i < numberOfThreads; i++)
            {
                bool first = blocksCandidates.at(i).mine(mined);

                if (first)
                    blocks.push_back(blocksCandidates.at(i));
            }
            // cout << "Block found" << endl;
            blocksCandidates.clear();

            allTime += blockTimer.Stop();

            this->printFormatedBlockInfo(blocks.back());
            this->removeAddedTransactions(blocks.back().getAllTransactions());
            this->updateUsersBalances(blocks.back().getAllTransactions());
        }
        cout << "Average block mine time: " << programTimer.Stop() / this->blocks.size() << endl;
        this->printBlocksToFile();
        this->printUsersToFile(users);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
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
    this->printFormatedBlockInfo(block);
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
            if (users.at(input.userPK).getBalance() < input.amount)
                userCanMakeTransaction = false;
        }

        if (!(*it).isTransactionHashValid() || !userCanMakeTransaction)
        {
            string txId = (*it).getTxID();
            transactions.erase(std::find_if(transactions.begin(), transactions.end(), [txId](const Transaction &tx)
                                            { return tx.getTxID() == txId; }));
            txs.erase(it);
            it--;
        }
    }
};
void Client::ajustDifficulty()
{
    if (this->blocks.size() > 0)
        difficulty += targetTime - allTime / this->blocks.size();
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
    return this->blocks.empty() ? "0000000000000000000000000000000000000000000000000000000000000000" : this->blocks.back().getBlockHash();
}
void Client::printFormatedBlockInfo(Block &block)
{
    cout << endl;
    cout << string(150, '-') << endl;
    cout << endl;

    cout << setw(15) << left << "Hash:" << setw(64) << right << block.getBlockHash() << setw(15) << left << "" << setw(20) << left << "Nonce:" << setw(20) << right << block.getNonce() << endl;
    cout << setw(15) << left << "Prev hash:" << setw(64) << right << block.getPrevHash() << setw(15) << left << "" << setw(20) << left << "Transaction count:" << setw(20) << right << block.getTransactionCount() << endl;
    cout << setw(15) << left << "Merkle hash:" << setw(64) << right << block.getMekleRootHash() << setw(15) << left << "" << setw(20) << left << "Time stamp:" << setw(20) << right << block.getTimestamp() << endl;
    cout << setw(15) << left << "Target hash:" << setw(64) << right << block.calcDifficultyTargetHash() << setw(15) << left << "" << setw(20) << left << "Difficulty:" << setw(20) << right << block.getDifficulty() << endl;
    cout << setw(15) << left << "" << setw(64) << right << "" << setw(15) << left << "" << setw(20) << left << "Mined by thread nr:" << setw(20) << right << block.getMinedThreadNumber() << endl;

    cout << endl;
    cout << string(150, '-') << endl;
    cout << endl;
};
void Client::updateUsersBalances(vector<Transaction> &transactionsToBlock)
{
    for (auto &tx : transactionsToBlock)
    {
        for (auto &in : tx.getInputs())
            users.at(in.userPK).updateBalance(in.amount);
        for (auto &out : tx.getOutputs())
            users.at(out.userPK).updateBalance(out.amount * -1);
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
