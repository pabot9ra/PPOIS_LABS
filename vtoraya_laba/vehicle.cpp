#include "vehicle.h"
#include "capacityexceededexception.h"
#include <cctype>
#include <stdexcept>

Vehicle::Vehicle(const std::string &plate, int maximumLoad, const Route &route)
    : plate_(plate), maximumLoad_(maximumLoad), route_(route), currentStop_(0), moving_(false) {
    if(maximumLoad_ <= 0 || route_.stopCount() < 2){
        throw std::invalid_argument("Vehicle requires positive capacity and a route with at least two stops");
    }
    bool hasPlate = false;
    for(char character : plate_){
        if(!std::isspace(static_cast<unsigned char>(character))){
            hasPlate = true;
            break;
        }
    }
    if(!hasPlate){
        throw std::invalid_argument("Vehicle plate must not be blank");
    }
}

const std::string &Vehicle::plate() const {
    return plate_;
}

int Vehicle::maximumLoad() const {
    return maximumLoad_;
}

int Vehicle::currentBranchId() const {
    return route_.stopAt(currentStop_).branchId();
}

bool Vehicle::moving() const {
    return moving_;
}

std::size_t Vehicle::bagCount() const {
    return bags_.size();
}

int Vehicle::loadGrams() const {
    int weight = 0;
    for(const MailBag &bag : bags_){
        weight += bag.weightGrams();
    }
    return weight;
}

int Vehicle::remainingCapacity() const {
    return maximumLoad_ - loadGrams();
}

void Vehicle::ensureCanLoad(const MailBag &bag) const {
    if(moving_ || !bag.sealed()){
        throw std::logic_error("Loading requires a stopped vehicle and a sealed bag");
    }
    if(bag.sourceBranchId() != currentBranchId() ||
       !route_.canReachAfter(currentStop_, bag.destinationBranchId())){
        throw std::invalid_argument("Bag source or destination does not match the remaining route");
    }
    if(bag.weightGrams() > remainingCapacity()){
        throw CapacityExceededException("Vehicle " + plate_, bag.weightGrams(), remainingCapacity());
    }
}

void Vehicle::ensureUniqueContents(const MailBag &bag) const {
    for(const MailBag &loaded : bags_){
        if(loaded.id() == bag.id()){
            throw std::invalid_argument("Bag is already loaded");
        }
        for(std::size_t i = 0; i < bag.shipmentCount(); i++){
            if(loaded.contains(bag.trackingNumberAt(i))){
                throw std::invalid_argument("Shipment is already loaded in another bag");
            }
        }
    }
}

void Vehicle::loadBag(const MailBag &bag) {
    ensureCanLoad(bag);
    ensureUniqueContents(bag);
    bags_.push_back(bag);
}

MailBag Vehicle::unloadBag(const std::string &bagId) {
    if(moving_){
        throw std::logic_error("Moving vehicle cannot be unloaded");
    }
    for(auto it = bags_.begin(); it != bags_.end(); ++it){
        if(it->id() == bagId){
            if(it->destinationBranchId() != currentBranchId()){
                throw std::logic_error("Bag can only be unloaded at its destination branch");
            }
            MailBag result = *it;
            bags_.erase(it);
            return result;
        }
    }
    throw std::out_of_range("Bag is not loaded");
}

void Vehicle::depart() {
    if(moving_ || currentStop_ + 1 >= route_.stopCount()){
        throw std::logic_error("Vehicle is already moving or has reached the end of its route");
    }
    for(const MailBag &bag : bags_){
        if(bag.destinationBranchId() == currentBranchId()){
            throw std::logic_error("Unload bags addressed to the current branch before departure");
        }
    }
    moving_ = true;
}

void Vehicle::arriveAt(int branchId) {
    if(!moving_){
        throw std::logic_error("Vehicle must depart before arrival");
    }
    if(route_.stopAt(currentStop_ + 1).branchId() != branchId){
        throw std::invalid_argument("Arrival branch must be the next route stop");
    }
    currentStop_++;
    moving_ = false;
}
