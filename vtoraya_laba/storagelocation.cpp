#include "storagelocation.h"
#include <cctype>
#include <stdexcept>

namespace {
bool isValidLocationId(const std::string &id) {
    if(id.empty()){
        return false;
    }
    for(char character : id){
        if(std::isspace(static_cast<unsigned char>(character))){
            return false;
        }
    }
    return true;
}
}

StorageLocation::StorageLocation(int branchId, const std::string &zoneId, const std::string &cellId)
    : branchId_(branchId), zoneId_(zoneId), cellId_(cellId) {
    if(branchId_ <= 0 || !isValidLocationId(zoneId_) || !isValidLocationId(cellId_)){
        throw std::invalid_argument("Storage location requires positive branch ID and zone/cell IDs without whitespace");
    }
}

int StorageLocation::branchId() const {
    return branchId_;
}

const std::string &StorageLocation::zoneId() const {
    return zoneId_;
}

const std::string &StorageLocation::cellId() const {
    return cellId_;
}

std::string StorageLocation::format() const {
    return std::to_string(branchId_) + "/" + zoneId_ + "/" + cellId_;
}

bool StorageLocation::operator==(const StorageLocation &other) const {
    return branchId_ == other.branchId_ && zoneId_ == other.zoneId_ && cellId_ == other.cellId_;
}
