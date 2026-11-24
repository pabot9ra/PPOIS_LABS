#include "invalidcustomerexception.h"
#include <UnitTest++/UnitTest++.h>
#include "customer.h"
#include <stdexcept>
#include <string>

TEST(CustomerStoresData) {
    PostalAddress address("Минск", "Гикало", "9", "220013");
    Customer customer(1, "Владислав", address);
    CHECK_EQUAL(1, customer.id());
    CHECK_EQUAL(std::string("Владислав"), customer.name());
    CHECK_EQUAL(address.formatLabel(), customer.address().formatLabel());
}

TEST(CustomerRejectsInvalidId) {
    PostalAddress address("Минск", "Гикало", "9", "220013");
    CHECK_THROW(Customer(0, "Владислав", address), InvalidCustomerException);
    CHECK_THROW(Customer(-1, "Владислав", address), InvalidCustomerException);
}

TEST(CustomerRejectsBlankName) {
    PostalAddress address("Минск", "Гикало", "9", "220013");
    const std::string names[] = {"", "   ", "\t\r\n\v\f"};
    for(const std::string &name : names){
        CHECK_THROW(Customer(1, name, address), InvalidCustomerException);
    }
}

TEST(CustomerChangesAddress) {
    PostalAddress original("Минск", "Гикало", "9", "220013");
    PostalAddress replacement("Брест", "Советская", "1", "224000");
    Customer customer(1, "Владислав", original);
    customer.changeAddress(replacement);
    CHECK_EQUAL(replacement.formatLabel(), customer.address().formatLabel());
    CHECK_EQUAL(1, customer.id());
    CHECK_EQUAL(std::string("Владислав"), customer.name());
    CHECK_EQUAL(std::string("Минск"), original.city());
}

TEST(CustomerOwnsCopyOfAddress) {
    PostalAddress address("Минск", "Гикало", "9", "220013");
    Customer customer(1, "Владислав", address);
    address = PostalAddress("Брест", "Советская", "1", "224000");
    CHECK_EQUAL(std::string("Минск"), customer.address().city());
}

TEST(CustomerCanKeepCurrentAddress) {
    PostalAddress address("Минск", "Гикало", "9", "220013");
    Customer customer(1, "Владислав", address);
    customer.changeAddress(customer.address());
    CHECK_EQUAL(address.formatLabel(), customer.address().formatLabel());
}
