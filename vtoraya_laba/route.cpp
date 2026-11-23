#include "route.h"
#include <cctype>
#include <limits>
#include <stdexcept>

Route::Route(int id, const std::string &name) : id_(id), name_(name) {
    if(id_ <= 0){
        throw std::invalid_argument("Route ID must be positive");
    }
    bool hasName = false;
    for(char character : name_){
        if(!std::isspace(static_cast<unsigned char>(character))){
            hasName = true;
            break;
        }
    }
    if(!hasName){
        throw std::invalid_argument("Route name must not be blank");
    }
}

int Route::id() const {
    return id_;
}

const std::string &Route::name() const {
    return name_;
}

std::size_t Route::stopCount() const {
    return stops_.size();
}

const RouteStop &Route::stopAt(std::size_t index) const {
    return stops_.at(index);
}

void Route::addStop(const RouteStop &stop) {
    if(stops_.empty() && stop.distanceKm() != 0){
        throw std::invalid_argument("First stop must have zero incoming distance");
    }
    if(!stops_.empty() && (stop.branchId() == stops_.back().branchId() ||
       stop.plannedMinute() <= stops_.back().plannedMinute())){
        throw std::invalid_argument("Next stop must be a different branch with a later planned time");
    }
    if(stop.distanceKm() > std::numeric_limits<int>::max() - totalDistanceKm()){
        throw std::overflow_error("Route distance is too large");
    }
    stops_.push_back(stop);
}

int Route::totalDistanceKm() const {
    int distance = 0;
    for(const RouteStop &stop : stops_){
        distance += stop.distanceKm();
    }
    return distance;
}

bool Route::canReachAfter(std::size_t currentStop, int branchId) const {
    if(currentStop >= stops_.size()){
        throw std::out_of_range("Current route stop does not exist");
    }
    for(std::size_t i = currentStop + 1; i < stops_.size(); i++){
        if(stops_[i].branchId() == branchId){
            return true;
        }
    }
    return false;
}
