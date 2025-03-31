#include <iostream>
#include <vector>
#include "Blockchain.h"
#include "Wallet.h"

int main() {
    Blockchain myBlockchain;
    std::vector<Wallet*> wallets;

    Wallet hamad("Hamad");
    Wallet bogdan("bogdan");

    wallets.push_back(&hamad);
    wallets.push_back(&bogdan);

    Transaction tx1 = hamad.sendFunds(bogdan, 50);  
    Transaction tx2 = bogdan.sendFunds(bogdan, 30); 

    myBlockchain.createTransaction(tx1);
    myBlockchain.createTransaction(tx2);

    myBlockchain.minePendingTransactions();

    myBlockchain.notifyWallets(wallets);

    if (myBlockchain.isChainValid()) {
        std::cout << "Blockchain is valid" << std::endl;
    } else {
        std::cout << "Blockchain is not valid!" << std::endl;
    }

    myBlockchain.printChain();

    for (const auto& wallet : wallets) {
        std::cout << "Wallet " << wallet->id << " has balance: " << wallet->balance << std::endl;
    }

    return 0;
}
