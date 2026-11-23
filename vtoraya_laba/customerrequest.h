#ifndef CUSTOMERREQUEST_H
#define CUSTOMERREQUEST_H

#include "shipment.h"
#include <memory>
#include <string>

enum class RequestStatus {
    Open,
    Approved,
    Rejected,
    Completed
};

class CustomerRequest {
private:
    int id_;
    Customer applicant_;
    std::string trackingNumber_;
    int openedMinute_;
    std::string reason_;
    RequestStatus status_;
    std::string decisionReason_;

protected:
    CustomerRequest(const CustomerRequest &other) = default;
    static bool hasText(const std::string &value);
    void ensureMatches(const Shipment &shipment) const;
    void ensureApproved() const;
    void markCompleted();
    virtual void validateEligibility(const Shipment &shipment) const = 0;

public:
    CustomerRequest(int id, const Customer &applicant, const Shipment &shipment,
                    int openedMinute, const std::string &reason);
    virtual ~CustomerRequest() = default;
    CustomerRequest &operator=(const CustomerRequest &other) = delete;

    int id() const;
    const Customer &applicant() const;
    const std::string &trackingNumber() const;
    int openedMinute() const;
    const std::string &reason() const;
    RequestStatus status() const;
    const std::string &decisionReason() const;
    void approve(const Shipment &shipment);
    void reject(const std::string &reason);
    virtual std::unique_ptr<CustomerRequest> clone() const = 0;
};

#endif
