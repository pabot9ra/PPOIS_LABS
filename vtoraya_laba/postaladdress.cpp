#include "postaladdress.h"

#include <cctype>
#include <cstddef>
#include <stdexcept>

namespace {
const std::size_t POSTAL_CODE_LENGTH = 6;

bool hasVisibleCharacters(const std::string &value) {
    for(std::size_t i = 0; i < value.size(); i++){
        if(!std::isspace(static_cast<unsigned char>(value[i]))){
            return true;
        }
    }
    return false;
}

bool isValidPostalCode(const std::string &postalCode) {
    if(postalCode.size() != POSTAL_CODE_LENGTH){
        return false;
    }
    for(std::size_t i = 0; i < postalCode.size(); i++){
        if(postalCode[i] < '0' || postalCode[i] > '9'){
            return false;
        }
    }
    return true;
}
}

PostalAddress::PostalAddress(const std::string &city, const std::string &street,
                             const std::string &house, const std::string &postalCode)
    : city_(city), street_(street), house_(house), postalCode_(postalCode) {
    if (!hasVisibleCharacters(city_) || !hasVisibleCharacters(street_) ||
        !hasVisibleCharacters(house_)) {
        throw std::invalid_argument("City, street and house must not be blank");
    }
    if (!isValidPostalCode(postalCode_)) {
        throw std::invalid_argument("Postal code must contain exactly six digits");
    }
}

const std::string &PostalAddress::city() const {
    return city_;
}

const std::string &PostalAddress::street() const {
    return street_;
}

const std::string &PostalAddress::house() const {
    return house_;
}

const std::string &PostalAddress::postalCode() const {
    return postalCode_;
}

std::string PostalAddress::formatLabel() const {
    return postalCode_ + ", " + city_ + ", " + street_ + ", " + house_;
}
