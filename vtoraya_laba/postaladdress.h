#ifndef POSTALADDRESS_H
#define POSTALADDRESS_H

#include <string>

class PostalAddress {
private:
    std::string city_;
    std::string street_;
    std::string house_;
    std::string postalCode_;

public:
    PostalAddress(const std::string &city, const std::string &street,
                  const std::string &house, const std::string &postalCode);

    const std::string &city() const;
    const std::string &street() const;
    const std::string &house() const;
    const std::string &postalCode() const;

    std::string formatLabel() const;
};

#endif
