#ifndef EXPRESSLETTER_H
#define EXPRESSLETTER_H

#include "letter.h"

class ExpressLetter : public Letter {
private:
    int deliveryHours_;
    int expressFee_;

public:
    ExpressLetter(const std::string &trackingNumber, const Customer &sender,
                  const Customer &recipient, int weightGrams, int pricePerGram,
                  int deliveryHours, int expressFee);

    int deliveryHours() const;
    int expressFee() const;
    bool isOverdue(int hoursInTransit) const;
    int calculatePrice() const override;
    std::unique_ptr<Shipment> clone() const override;
};

#endif
