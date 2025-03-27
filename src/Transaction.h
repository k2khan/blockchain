#pragma once
#include <string>
#include <openssl/rsa.h>

class Transaction {
public:
    std::string sender; // Sender's wallet id
    std::string receiver; // Receiver's wallet id
    float amount; // Amount of money transferred
    std::string signature; // Digital signature for this transaction
    int nonce; // Nonce for this transaction, can be used for security or versioning
    unsigned int signatureLength; // Length of the digital signature

    // Constructor to initialize the transaction
    Transaction(std::string sender, std::string reeiver, float amount, int nonce);

    void sign(RSA* privateKey);

    bool verify(RSA* publicKey) const;

    bool isValid(RSA* publicKey) const;
};
