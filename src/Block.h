#pragma once
#include <string>
#include <vector>
#include <ctime>
#include "Transaction.h"

class Block {
public:
    std::string prevHash; // Previous block's hash
    std::string blockHash; // Current block's hash
    std::vector<Transaction> transactions; // List of transactions in this block
    std::time_t timestamp; // Block creation timestamp
    int nonce; // Nonce used for mining
    int difficulty; // Difficulty level of mining this block

    Block(std::vector<Transaction> transactions, std::string prevHash, int difficulty);

    // Method to mine this block by finding a nonce that satisfies the difficulty
    std::string mineBlock(); 

    // Method to generate the hash of the block's contents
    std::string generateHash() const;

    // Utility method to perform SHA-256 hashing
    std::string sha256(const std::string str) const;
};
