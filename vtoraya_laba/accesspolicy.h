#ifndef ACCESSPOLICY_H
#define ACCESSPOLICY_H

#include <cstddef>
#include <set>
#include <string>

class AccessPolicy {
private:
    std::set<std::string> permissions_;
    bool enabled_;
    std::size_t deniedCount_;

public:
    AccessPolicy();

    bool enabled() const;
    std::size_t deniedCount() const;
    bool hasPermission(const std::string &permission) const;
    void grantPermission(const std::string &permission);
    void revokePermission(const std::string &permission);
    void checkPermission(const std::string &permission);
    void disable();
    void enable();
};

#endif
