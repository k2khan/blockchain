#include <iostream>
#include <vector>
#include "Blockchain.h"
#include "Wallet.h"

int main() {
    Blockchain myBlockchain;
    std::vector<Wallet*> wallets;

    Wallet hamad("Hamad");
    Wallet humza("Humza");

    hamad.balance = 100;
    humza.balance = 100;

    wallets.push_back(&hamad);
    wallets.push_back(&humza);

    Transaction tx1 = hamad.sendFunds(humza, 50);  
    Transaction tx2 = humza.sendFunds(humza, 30); 

    myBlockchain.createTransaction(tx1);
    myBlockchain.createTransaction(tx2);

    myBlockchain.minePendingTransactions();

    myBlockchain.notifyWallets(wallets);

    if (myBlockchain.isChainValid()) {
        std::cout << "Blockchain is valid.\n";
    } else {
        std::cout << "Blockchain is not valid!\n";
    }

    myBlockchain.printChain();

    for (const auto& wallet : wallets) {
        std::cout << "Wallet " << wallet->id << " has balance: " << wallet->balance << std::endl;
    }

    return 0;
}
