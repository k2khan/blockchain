#include "Blockchain.h"
#include "Wallet.h"
#include <iostream>

Blockchain::Blockchain() {
    std::vector<Transaction> emptyTransactions;
    chain.emplace_back(emptyTransactions, "0", 2);
}

bool Blockchain::createTransaction(Transaction transaction) {
    if (transaction.amount <= 0) {
        std::cerr << "Transaction rejected: Amount must be positive." << std::endl;
        return false;
    }

    if (transaction.receiver == transaction.sender) {
        std::cerr << "Transaction rejected: Transaction receiver can not be the receiver." << std::endl;
        return false;
    }

    if (getBalance(transaction.sender) < transaction.amount) {
        std::cerr << "Transaction rejected. Sender: (" << transaction.sender << ") has insufficient funds. Balance: " << getBalance(transaction.sender) << ". Transaction amount: " << transaction.amount << std::endl; :
        return false;
    }

    pendingTransactions.push_back(transaction);
    std::cout << "Transaction added to pending pool." << std::endl;
    return true;
}

void Blockchain::minePendingTransactions(/* std::string minerAddress */) {
    // Add miner reward transaction here later:
    // Transaction rewardTx("System", minerAddress, MINING_REWARD, time(0)); // Nonce might need care
    // std::vector<Transaction> blockTransactions = {rewardTx}; 
    // blockTransactions.insert(blockTransactions.end(), pendingTransactions.begin(), pendingTransactions.end());
    // Block newBlock(blockTransactions, ...);

    if (pendingTransactions.empty()) {
        std::cout << "No pending transactions to mine." << std::endl;
        // Optionally mine an empty block or just return
        return; 
    }

    Block newBlock(pendingTransactions, chain.back().blockHash, 2);
    chain.push_back(newBlock);
    updateBalanceFromBlock(newBlock);

    std::cout << "Block successfully mined. Hash: " << newBlock.blockHash << std::endl;
    
    pendingTransactions.clear();
}

bool Blockchain::isBlockHashValid(const Block& block) {
    return block.blockHash == block.generateHash();
}


bool Blockchain::isTransactionValid(const Transaction& tx) {
    return tx.amount > 0;
}

bool Blockchain::isChainValid() {
    for (int i = 1; i < chain.size(); i++) {
        Block currBlock = chain[i];
        Block prevBlock = chain[i-1];

        if (!isBlockHashValid(currBlock)) {
            return false;
        }
        
        if (currBlock.prevHash != prevBlock.blockHash) {
            return false;
        }

        for (const auto& tx : currBlock.transactions) {
            RSA* publicKey = publicKeyMap[tx.sender];
            if (!tx.isValid(publicKey)) {
                return false;
            }
        }
    }
    return true;
}

void Blockchain::printChain() {
    for (const auto& block : chain) {
        std::cout << "Block Timestamp: " << block.timestamp << std::endl;
        std::cout << "Previous Hash: " << block.prevHash << std::endl;
        std::cout << "Block Hash: " << block.blockHash << std::endl;
        std::cout << "Transactions:" << std::endl;

        for (const auto& tx : block.transactions) {
            std::cout << "  Sender: " << tx.sender << " Receiver: " << tx.receiver << " Amount: " << tx.amount << std::endl;
        }

        std::cout << "Nonce: " << block.nonce << std::endl;
        std::cout << std::endl;
    }
}

void Blockchain::notifyWallets(std::vector<Wallet*>& wallets) {
    for (auto& wallet : wallets) {
        publicKeyMap[wallet->id] = wallet->publicKey;
        for (auto& block : chain) {
            wallet->updateBalance(block.transactions);
        }
    }
}

void Blockchain::updateBalancesFromBlock(const Block& block) {
    for (const auto& tx: block.transactions) {
        if (balances.find(tx.sender) == balances.end()) {
            balances[tx.sender] = 0.0f;
        }
        if (balances.find(tx.receiver) == balances.end()) {
            balances[tx.receiver] = 0.0f;
        }

        balances[tx.sender] -= tx.amount;
        balances[tx.receiver] -= tx.amount;
    }
}

float Blockchain::getBalance(const std::string& string) const {
    if (publicKeyMap.find(string) == publicKeyMap.end()) {
        return -1.0f;
    } else {
        return public
    }

}