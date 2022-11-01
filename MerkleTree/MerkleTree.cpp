#include "MerkleTree.h"

void MerkleTree::addTransaction(string tx)
{
    string txHash = this->hasher.hashString(tx);
    this->leaves[tx] = new Node(txHash);
};
// bool MerkleTree::validateTransaction(string txId)
// {
//     Node *txToValidate = this->findTransaction(txId);
// };
void MerkleTree::genMerkelTree()
{
    if (this->leaves.size() == 0)
    {
        string hash(64, '0');
        this->root = new Node(hash);
        return;
    }

    vector<Node *> botLine;
    for (auto &el : this->leaves)
        botLine.push_back(el.second);
    this->root = buildMerkelTree(botLine);
};
string MerkleTree::getRootHash()
{
    return this->root->hash;
};
Node *MerkleTree::getRoot()
{
    return this->root;
};

Node *MerkleTree::buildMerkelTree(vector<Node *> &line)
{
    if (line.empty())
        return nullptr;

    if (line.size() == 1)
        return line.at(0);

    if (line.size() % 2 != 0)
        line.push_back(line.back());

    vector<Node *> topLine;

    string concatedHashes;
    string newHash;
    Node *tNode;
    for (auto it = line.begin(); it != line.end(); it += 2)
    {
        concatedHashes = (**it).hash + (**(it + 1)).hash;
        newHash = this->hasher.hashString(concatedHashes);
        tNode = new Node(newHash);

        tNode->left = *it;
        tNode->right = *(it + 1);

        (**it).parent = tNode;
        (**(it + 1)).parent = tNode;

        topLine.push_back(tNode);
    }

    return this->buildMerkelTree(topLine);
}
