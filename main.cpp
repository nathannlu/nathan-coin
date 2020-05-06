/*
 * Main
 * Nathan Coin Blockchain
 * 
 * Author: Nathan Lu
 * Created: 05/05/20
 * Edited: 05/06/20
 * 
 */

#include <iostream>
#include <ctime>
#include <vector>
#include <string>

using namespace std;

// Transaction data
struct TransactionData {
  double amount;
  string senderKey;
  string receiverKey;
  time_t timestamp;
};

// Block class
class Block {
  private:
    int index;
    size_t blockHash;
    size_t previousHash;
    size_t generateHash();

  public:
    // Constructor
    Block(int idx, TransactionData d, size_t prevHash);

    // Get original hash
    size_t getHash();

    // Get previous hash
    size_t getPreviousHash();

    // Transaction data
    TransactionData data;

    // Validate hash
    bool isHashValid();
};

// Constructor with params
Block::Block(int idx, TransactionData d, size_t prevHash) {
  index = idx;
  data = d;
  previousHash = prevHash;
  blockHash = generateHash();
};

// Private functions
size_t Block::generateHash() {
  hash<string> hash1;
  hash<size_t> hash2;
  hash<size_t> finalHash;
  string toHash = to_string(data.amount) + data.receiverKey + data.senderKey + to_string(data.timestamp);

  return finalHash(hash1(toHash) + hash2(previousHash));
};

// Public functions
size_t Block::getHash() {
  return blockHash;
}; 

size_t Block::getPreviousHash() {
  return previousHash;
};

bool Block::isHashValid() {
  return generateHash() == blockHash;
};

// Blockchain class
class Blockchain {
  private:
    Block createGenesisBlock();

  public:
    // Public chain
    vector<Block> chain;

    // Constructor
    Blockchain();

    // Public functions
    void addBlock(TransactionData data);
    bool isChainValid();

    // Contrived example for demo
    Block *getLatestBlock();
};

// Blockchain constructor
Blockchain::Blockchain() {
  Block genesis = createGenesisBlock();
  chain.push_back(genesis);
};

Block Blockchain::createGenesisBlock() {
  time_t current;
  TransactionData d;
  d.amount = 0;
  d.receiverKey = "None";
  d.senderKey = "None";
  d.timestamp = time(&current);

  hash<int> hash1;
  Block genesis(0, d, hash1(0));
  return genesis;
};

Block *Blockchain::getLatestBlock() {
  return &chain.back();
};

void Blockchain::addBlock(TransactionData d) {
  int index = (int)chain.size() - 1;
  Block newBlock(index, d, getLatestBlock() -> getHash());
}

bool Blockchain::isChainValid() {
  vector<Block>::iterator it;
  int chainLen = (int)chain.size();

  for(it = chain.begin(); it != chain.end(); ++it) {
    Block currentBlock = *it;
    if(!currentBlock.isHashValid()) {
      return false;
    }

    if(chainLen > 1) {
      Block previousBlock = *(it - 1);
      if(currentBlock.getPreviousHash() != previousBlock.getHash()) {
        return false;
      }
    }
  }

  return true;
};

int main() {
  // Start
  Blockchain NathanCoin;
  
  // Data for first added block
  TransactionData data1;
  time_t data1Time;
  data1.amount = 1.5;
  data1.receiverKey = "jeff";
  data1.senderKey = "Nathan";
  data1.timestamp = time(&data1Time);

  NathanCoin.addBlock(data1);

  cout << "Is chain valid?" << endl
      << NathanCoin.isChainValid() << endl;
}