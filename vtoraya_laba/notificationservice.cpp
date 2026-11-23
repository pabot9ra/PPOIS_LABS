#include "notificationservice.h"
#include <limits>
#include <stdexcept>

NotificationService::NotificationService(const PostalBranch &branch)
    : branchId_(branch.id()), nextId_(1) {
}

int NotificationService::branchId() const {
    return branchId_;
}

std::size_t NotificationService::noticeCount() const {
    return notices_.size();
}

int NotificationService::createNotice(const PostalBranch &branch, const std::string &trackingNumber,
                                      const std::string &contact) {
    if(branch.id() != branchId_){
        throw std::invalid_argument("Notification service belongs to another branch");
    }
    if(nextId_ == std::numeric_limits<int>::max()){
        throw std::overflow_error("Notice IDs are exhausted");
    }
    DeliveryNotice notice(nextId_, branch, trackingNumber, contact);
    notices_.push_back(notice);
    return nextId_++;
}

const DeliveryNotice &NotificationService::findNotice(int id) const {
    for(const DeliveryNotice &notice : notices_){
        if(notice.id() == id){
            return notice;
        }
    }
    throw std::out_of_range("Delivery notice was not found");
}

DeliveryNotice &NotificationService::findMutableNotice(int id) {
    for(DeliveryNotice &notice : notices_){
        if(notice.id() == id){
            return notice;
        }
    }
    throw std::out_of_range("Delivery notice was not found");
}

void NotificationService::markSent(int id) {
    DeliveryNotice &notice = findMutableNotice(id);
    notice.markSent();
}

void NotificationService::markRead(int id) {
    DeliveryNotice &notice = findMutableNotice(id);
    notice.markRead();
}
