#include <UnitTest++/UnitTest++.h>
#include "box.h"
#include "envelope.h"
#include "letter.h"
#include "parcel.h"
#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>

static_assert(std::is_abstract<Packaging>::value, "Packaging must be abstract");
static_assert(std::has_virtual_destructor<Packaging>::value, "Packaging destructor must be virtual");

TEST(DimensionsStoresMillimetersAndComputesVolume) {
    Dimensions dimensions(10, 20, 30);
    CHECK_EQUAL(10, dimensions.length());
    CHECK_EQUAL(20, dimensions.width());
    CHECK_EQUAL(30, dimensions.height());
    CHECK_EQUAL(6000LL, dimensions.volume());
}

TEST(DimensionsRejectsNonpositiveSides) {
    CHECK_THROW(Dimensions(0, 20, 30), std::invalid_argument);
    CHECK_THROW(Dimensions(10, -1, 30), std::invalid_argument);
    CHECK_THROW(Dimensions(10, 20, 0), std::invalid_argument);
}

TEST(DimensionsChecksVolumeOverflow) {
    int maximum = std::numeric_limits<int>::max();
    CHECK_THROW(Dimensions(maximum, maximum, 3), std::overflow_error);
    Dimensions large(maximum, maximum, 2);
    CHECK_EQUAL(static_cast<long long>(maximum) * maximum * 2, large.volume());
}

TEST(DimensionsTriesAllSixOrthogonalOrientations) {
    Dimensions container(4, 5, 6);
    const int sides[][3] = {{4,5,6},{4,6,5},{5,4,6},{5,6,4},{6,4,5},{6,5,4}};
    for(int i = 0; i < 6; i++){
        Dimensions contents(sides[i][0], sides[i][1], sides[i][2]);
        CHECK(contents.fitsInside(container));
    }
    CHECK(!Dimensions(1, 1, 7).fitsInside(container));
    CHECK(!Dimensions(5, 5, 5).fitsInside(container));
}

TEST(DimensionsInsetsBothSidesAndPreservesOriginal) {
    Dimensions outer(100, 80, 60);
    Dimensions inner = outer.inset(10);
    CHECK_EQUAL(80, inner.length());
    CHECK_EQUAL(60, inner.width());
    CHECK_EQUAL(40, inner.height());
    CHECK_EQUAL(100, outer.length());
    CHECK_EQUAL(100, outer.inset(0).length());
    CHECK_THROW(outer.inset(-1), std::invalid_argument);
    CHECK_THROW(outer.inset(30), std::invalid_argument);
    CHECK_THROW(outer.inset(std::numeric_limits<int>::max()), std::invalid_argument);
}

TEST(EnvelopeLabelsReflectProtectionOptions) {
    Envelope plain(Dimensions(200, 100, 10), 100, 20, false, false);
    Envelope protectedEnvelope(Dimensions(200, 100, 10), 100, 20, true, true);
    const Packaging &base = protectedEnvelope;
    CHECK_EQUAL(std::string("ENVELOPE"), plain.label());
    CHECK_EQUAL(std::string("ENVELOPE; REINFORCED; WATERPROOF"), base.label());
    CHECK(!plain.reinforced());
    CHECK(!plain.waterproof());
    CHECK(protectedEnvelope.reinforced());
    CHECK(protectedEnvelope.waterproof());
}

TEST(BoxAccountsForPaddingAndLabelsMaterial) {
    Box box(Dimensions(100, 80, 60), 5000, 150, 10, "Cardboard");
    CHECK_EQUAL(10, box.padding());
    CHECK_EQUAL(std::string("Cardboard"), box.material());
    CHECK_EQUAL(80, box.innerDimensions().length());
    CHECK_EQUAL(60, box.innerDimensions().width());
    CHECK_EQUAL(40, box.innerDimensions().height());
    Dimensions outer = box.outerDimensions();
    CHECK_EQUAL(100, outer.length());
    CHECK_EQUAL(80, outer.width());
    CHECK_EQUAL(60, outer.height());
    const Packaging &base = box;
    CHECK_EQUAL(std::string("BOX; Cardboard; PADDING 10 mm"), base.label());
}

TEST(BoxRejectsBlankMaterialOrImpossiblePadding) {
    CHECK_THROW(Box(Dimensions(100, 80, 60), 5000, 150, 10, ""), std::invalid_argument);
    CHECK_THROW(Box(Dimensions(100, 80, 60), 5000, 150, 10, " \t\n"), std::invalid_argument);
    CHECK_THROW(Box(Dimensions(100, 80, 60), 5000, 150, 30, "Cardboard"), std::invalid_argument);
    CHECK_THROW(Box(Dimensions(100, 80, 60), 5000, 150, -1, "Cardboard"), std::invalid_argument);
}

TEST(PackagingValidatesLimitsAndInitialState) {
    CHECK_THROW(Envelope(Dimensions(200,100,10), 0, 20, false, false), std::invalid_argument);
    CHECK_THROW(Envelope(Dimensions(200,100,10), -1, 20, false, false), std::invalid_argument);
    CHECK_THROW(Envelope(Dimensions(200,100,10), 100, -1, false, false), std::invalid_argument);
    Envelope envelope(Dimensions(200,100,10), 100, 0, false, false);
    CHECK_EQUAL(100, envelope.maximumWeight());
    CHECK_EQUAL(0, envelope.price());
    CHECK(envelope.trackingNumber().empty());
    CHECK(!envelope.sealed());
}

struct PackagingFixture {
    PostalAddress address{"Минск", "Гикало", "9", "220013"};
    Customer sender{1, "Владислав", address};
    Customer recipient{2, "Алексей", address};
    Letter letter{"LT001", sender, recipient, 20, 5};
    Parcel parcel{"PK001", sender, recipient, 1500, 200, 100};
    Envelope envelope{Dimensions(200, 100, 10), 100, 20, false, false};
    Box box{Dimensions(200, 200, 200), 2000, 100, 10, "Cardboard"};
};

TEST_FIXTURE(PackagingFixture, PackagingSupportsFillSealOpenAndUnpack) {
    envelope.pack(letter, Dimensions(180, 90, 5));
    CHECK_EQUAL(std::string("LT001"), envelope.trackingNumber());
    CHECK(!envelope.sealed());
    envelope.seal();
    CHECK(envelope.sealed());
    CHECK_THROW(envelope.unpack(), std::logic_error);
    CHECK_THROW(envelope.seal(), std::logic_error);
    envelope.open();
    CHECK(!envelope.sealed());
    envelope.unpack();
    CHECK(envelope.trackingNumber().empty());
}

TEST_FIXTURE(PackagingFixture, EmptyPackagingRejectsInvalidOperations) {
    CHECK_THROW(envelope.seal(), std::logic_error);
    CHECK_THROW(envelope.open(), std::logic_error);
    CHECK_THROW(envelope.unpack(), std::logic_error);
    CHECK(envelope.trackingNumber().empty());
}

TEST_FIXTURE(PackagingFixture, FilledPackagingCannotBePackedTwice) {
    envelope.pack(letter, Dimensions(180,90,5));
    CHECK_THROW(envelope.pack(letter, Dimensions(180,90,5)), std::logic_error);
    CHECK_EQUAL(std::string("LT001"), envelope.trackingNumber());
}

TEST_FIXTURE(PackagingFixture, PackagingRejectsWeightAndDimensionExcess) {
    CHECK_THROW(envelope.pack(parcel, Dimensions(10,10,10)), std::invalid_argument);
    CHECK_THROW(envelope.pack(letter, Dimensions(201,1,1)), std::invalid_argument);
    CHECK(envelope.trackingNumber().empty());
    CHECK(!envelope.sealed());
    CHECK_THROW(box.pack(parcel, Dimensions(190,190,190)), std::invalid_argument);
    CHECK(box.trackingNumber().empty());
}

TEST_FIXTURE(PackagingFixture, PackagingAcceptsExactWeightAndRotatedSizeLimits) {
    Envelope exact(Dimensions(100,200,10), 20, 20, false, false);
    exact.pack(letter, Dimensions(200,100,10));
    CHECK_EQUAL(std::string("LT001"), exact.trackingNumber());
}

TEST_FIXTURE(PackagingFixture, PackagingRequiresNewlyAcceptedShipment) {
    letter.dispatch();
    CHECK_THROW(envelope.pack(letter, Dimensions(180,90,5)), std::logic_error);
    letter.deliver();
    CHECK_THROW(envelope.pack(letter, Dimensions(180,90,5)), std::logic_error);
    CHECK(envelope.trackingNumber().empty());
}

TEST_FIXTURE(PackagingFixture, PackagingUsesPolymorphicShipmentWeight) {
    const Shipment &baseLetter = letter;
    const Shipment &baseParcel = parcel;
    CHECK_EQUAL(20, baseLetter.weightGrams());
    CHECK_EQUAL(1500, baseParcel.weightGrams());
    box.pack(baseParcel, Dimensions(180,180,180));
    CHECK_EQUAL(std::string("PK001"), box.trackingNumber());
}
