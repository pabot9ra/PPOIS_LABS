#include "invalidaddressexception.h"
#include <UnitTest++/UnitTest++.h>

#include "postaladdress.h"

#include <stdexcept>
#include <string>

TEST(AddressStoresFields) {
    PostalAddress address("Минск", "Гикало", "9", "220013");
    CHECK_EQUAL(std::string("Минск"), address.city());
    CHECK_EQUAL(std::string("Гикало"), address.street());
    CHECK_EQUAL(std::string("9"), address.house());
    CHECK_EQUAL(std::string("220013"), address.postalCode());
}

TEST(AddressFormatsLabel) {
    PostalAddress address("Минск", "Гикало", "9", "220013");
    CHECK_EQUAL(std::string("220013, Минск, Гикало, 9"), address.formatLabel());
}

TEST(AddressAcceptsHouseWithLetter) {
    PostalAddress address("Минск", "Гикало", "12А", "220013");
    CHECK_EQUAL(std::string("12А"), address.house());
}

TEST(AddressPreservesLeadingZeroInPostalCode) {
    PostalAddress address("Город", "Улица", "1", "012345");
    CHECK_EQUAL(std::string("012345"), address.postalCode());
}

TEST(AddressRejectsEmptyOrBlankFields) {
    const std::string blankValues[] = {"", "   ", "\t\n"};
    for (const std::string &blank : blankValues) {
        CHECK_THROW(PostalAddress(blank, "Гикало", "9", "220013"), InvalidAddressException);
        CHECK_THROW(PostalAddress("Минск", blank, "9", "220013"), InvalidAddressException);
        CHECK_THROW(PostalAddress("Минск", "Гикало", blank, "220013"), InvalidAddressException);
    }
}

TEST(AddressRejectsInvalidPostalCode) {
    const std::string invalidCodes[] = {"", "22001", "2200130", "22001A", "220 13", " 220013", "220013 "};
    for (const std::string &postalCode : invalidCodes) {
        CHECK_THROW(PostalAddress("Минск", "Гикало", "9", postalCode), InvalidAddressException);
    }
}

