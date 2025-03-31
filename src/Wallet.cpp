#include "Wallet.h"
#include <iostream>
#include <openssl/rand.h>
#include <openssl/pem.h>

Wallet::Wallet(std::string id) : id(id), balance(0.0f), nonce(0), publicKey(nullptr), privateKey(nullptr) {
    generateKeys();
}

Wallet::~Wallet() {
    if (privateKey) {
        RSA_free(privateKey);
        privateKey = nullptr;
    }

    if (publicKey) {
        RSA_free(publicKey);
        publicKey = nullptr;
    }
}


void Wallet::generateKeys() {
    privateKey = RSA_new();
    publicKey = RSA_new();
    BIGNUM* exponent = BN_new();
    BN_set_word(exponent, RSA_F4);
    RSA_generate_key_ex(privateKey, 2048, exponent, nullptr);
    RSA_set0_key(publicKey, BN_dup(RSA_get0_n(privateKey)), BN_dup(exponent), nullptr);

    BN_free(exponent);

    char* publicKeyStr = nullptr;
    BIO *bio = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(bio, publicKey);
    size_t keylen = BIO_pending(bio);
    publicKeyStr = (char*)malloc(keylen + 1);
    BIO_read(bio, publicKeyStr, keylen);
    publicKeyStr[keylen] = 0;
    BIO_free_all(bio);


    free(publicKeyStr); 
}

Transaction Wallet::sendFunds(Wallet& receiver, float amount) {
    nonce++;
    Transaction tx(id, receiver.id, amount, nonce);
    tx.sign(privateKey);
    std::cout << privateKey << std::endl;

    return tx;
}

void Wallet::updateBalance(const std::vector<Transaction>& transactions) {
    for (const auto& tx : transactions) {
        if (tx.sender == id) {
            balance -= tx.amount;
        }
        if (tx.receiver == id) {
                balance += tx.amount;
        }
    }
}

