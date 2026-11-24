#include "accesspolicy.h"
#include "accessdeniedexception.h"
#include <cctype>
#include <stdexcept>

AccessPolicy::AccessPolicy() : enabled_(true), deniedCount_(0) {
}

bool AccessPolicy::enabled() const {
    return enabled_;
}

std::size_t AccessPolicy::deniedCount() const {
    return deniedCount_;
}

bool AccessPolicy::hasPermission(const std::string &permission) const {
    return permissions_.count(permission) > 0;
}

void AccessPolicy::grantPermission(const std::string &permission) {
    bool hasName = false;
    for(char character : permission){
        if(!std::isspace(static_cast<unsigned char>(character))){
            hasName = true;
            break;
        }
    }
    if(!hasName){
        throw std::invalid_argument("Permission name must not be blank");
    }
    permissions_.insert(permission);
}

void AccessPolicy::revokePermission(const std::string &permission) {
    if(permissions_.erase(permission) == 0){
        throw std::invalid_argument("Permission was not granted");
    }
}

void AccessPolicy::checkPermission(const std::string &permission) {
    if(!enabled_ || !hasPermission(permission)){
        deniedCount_++;
        throw AccessDeniedException(permission);
    }
}

void AccessPolicy::disable() {
    if(!enabled_){
        throw std::logic_error("Access policy is already disabled");
    }
    enabled_ = false;
}

void AccessPolicy::enable() {
    if(enabled_){
        throw std::logic_error("Access policy is already enabled");
    }
    enabled_ = true;
}
