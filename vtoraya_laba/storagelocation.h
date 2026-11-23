#ifndef STORAGELOCATION_H
#define STORAGELOCATION_H

#include <string>

class StorageLocation {
private:
    int branchId_;
    std::string zoneId_;
    std::string cellId_;

public:
    StorageLocation(int branchId, const std::string &zoneId, const std::string &cellId);

    int branchId() const;
    const std::string &zoneId() const;
    const std::string &cellId() const;
    std::string format() const;
    bool operator==(const StorageLocation &other) const;
};

#endif
