#include "deliverynotice.h"
#include <cctype>
#include <stdexcept>

namespace {
bool hasContact(const std::string &contact) {
    for(char character : contact){
        if(!std::isspace(static_cast<unsigned char>(character))){
            return true;
        }
    }
    return false;
}
}

DeliveryNotice::DeliveryNotice(int id, const PostalBranch &branch, const std::string &trackingNumber,
                               const std::string &contact)
    : id_(id), trackingNumber_(trackingNumber), contact_(contact), branchId_(branch.id()),
      branchAddress_(branch.address().formatLabel()), sent_(false), read_(false) {
    if(id_ <= 0 || !hasContact(contact_)){
        throw std::invalid_argument("Notice requires positive ID and nonblank contact");
    }
    const Shipment &shipment = branch.findShipment(trackingNumber_);
    if(shipment.status() != ShipmentStatus::Delivered){
        throw std::logic_error("Delivery notice requires a delivered shipment");
    }
    recipientName_ = shipment.recipient().name();
}

int DeliveryNotice::id() const {
    return id_;
}

const std::string &DeliveryNotice::trackingNumber() const {
    return trackingNumber_;
}

const std::string &DeliveryNotice::contact() const {
    return contact_;
}

int DeliveryNotice::branchId() const {
    return branchId_;
}

bool DeliveryNotice::sent() const {
    return sent_;
}

bool DeliveryNotice::read() const {
    return read_;
}

std::string DeliveryNotice::formatMessage() const {
    return recipientName_ + ", shipment " + trackingNumber_ + " was delivered at branch " +
           std::to_string(branchId_) + ": " + branchAddress_;
}

void DeliveryNotice::markSent() {
    if(sent_){
        throw std::logic_error("Notice is already marked as sent");
    }
    sent_ = true;
}

void DeliveryNotice::markRead() {
    if(!sent_ || read_){
        throw std::logic_error("Only a sent, unread notice can be marked as read");
    }
    read_ = true;
}
