#pragma once
#include <vector>
#include <unordered_map>
#include "Block.h"
#include "Transaction.h"

class Blockchain {
private:
    std::vector<Block> chain;
    std::vector<Transaction> pendingTransactions;
    std::unordered_map<std::string, RSA*> publicKeyMap;
    std::unordered_map<std::string, float> balances;

public:
    Blockchain();

    bool createTransaction(Transaction transaction);

    void minePendingTransactions();

    bool isBlockHashValid(const Block& block);

    bool isTransactionValid(const Transaction& tx);

    bool isChainValid();

    void printChain();

    void updateBalanceFromBlock(const Block& block);

    float getBalance(const std::string& address) const;
};
