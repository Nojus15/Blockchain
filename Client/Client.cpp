#include "Client.h"

void Client::startMining()
{
    vector<Transaction> allTransactions(this->getAllTransactions());
    int difficulty = 0;
    string target, merkleHash, timestamp, prevHash, guess;
    int nonce = 0;
    string version = "1";

    int count, pos;
    while (allTransactions.size() > 0)
    {
        count = gen.genInt(1, allTransactions.size() > 100 ? 100 : allTransactions.size());
        pos = gen.genInt(0, allTransactions.size() - count);

        vector<Transaction> txsToBlock(allTransactions.begin() + pos, allTransactions.begin() + pos + count);

        MerkleTree *merkleBuilder = new MerkleTree();
        for (auto &el : txsToBlock)
            merkleBuilder->addTransaction(el.getTxID());
        merkleBuilder->genMerkelTree();

        target = this->getTargetHash(difficulty);
        timestamp = std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
        merkleHash = merkleBuilder->getRootHash();
        prevHash = this->blocks.empty() ? "0000000000000000000000000000000000000000000000000000000000000000" : this->blocks.back().getBlockHash();

        while (target < (guess = this->hasher.hashString(prevHash + timestamp + version + to_string(nonce) + merkleHash + to_string(difficulty))))
        {
            if (nonce % 1000 == 0)
            {
                cout << "Nonce: " << nonce << " Guess: " << guess << '\r';
                timestamp = std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
            }
            nonce++;
        }
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
    }
    this->printBlocksToFile();
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
string Client::getTargetHash(int difficulty)
{
    string target(64, '7');
    for (int i = 0; i < difficulty; i++)
    {
        target[i] = '0';
    }
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