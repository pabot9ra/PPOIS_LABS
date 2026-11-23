#ifndef REDIRECTIONREQUEST_H
#define REDIRECTIONREQUEST_H

#include "customerrequest.h"

class RedirectionRequest : public CustomerRequest {
private:
    PostalAddress destination_;
    int redirectionFee_;

protected:
    void validateEligibility(const Shipment &shipment) const override;

public:
    RedirectionRequest(int id, const Customer &applicant, const Shipment &shipment,
                       int openedMinute, const std::string &reason,
                       const PostalAddress &destination, int redirectionFee);

    const PostalAddress &destination() const;
    int redirectionFee() const;
    std::string issueLabel(const Shipment &shipment);
    std::unique_ptr<CustomerRequest> clone() const override;
};

#endif
