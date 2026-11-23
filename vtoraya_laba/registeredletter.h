#ifndef REGISTEREDLETTER_H
#define REGISTEREDLETTER_H

#include "letter.h"

class RegisteredLetter : public Letter {
private:
    int registrationFee_;
    std::string signedBy_;

public:
    RegisteredLetter(const std::string &trackingNumber, const Customer &sender,
                     const Customer &recipient, int weightGrams, int pricePerGram,
                     int registrationFee);

    int registrationFee() const;
    const std::string &signedBy() const;
    void confirmReceipt(const std::string &signer);
    int calculatePrice() const override;
    std::unique_ptr<Shipment> clone() const override;
};

#endif
