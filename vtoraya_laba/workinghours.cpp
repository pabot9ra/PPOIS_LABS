#include "workinghours.h"
#include <stdexcept>

const int WorkingHours::MINUTES_PER_DAY;

WorkingHours::WorkingHours(int opensAt, int closesAt, int breakStart, int breakEnd)
    : opensAt_(opensAt), closesAt_(closesAt), breakStart_(breakStart), breakEnd_(breakEnd) {
    if(opensAt_ < 0 || opensAt_ >= closesAt_ || closesAt_ > MINUTES_PER_DAY ||
       breakStart_ < opensAt_ || breakStart_ > breakEnd_ || breakEnd_ > closesAt_){
        throw std::invalid_argument("Opening hours and break must form valid intervals within one day");
    }
    if(workingMinutes() == 0){
        throw std::invalid_argument("Schedule must contain working time outside the break");
    }
}

void WorkingHours::validateMinute(int minute) {
    if(minute < 0 || minute >= MINUTES_PER_DAY){
        throw std::invalid_argument("Minute of day is outside the valid range");
    }
}

int WorkingHours::opensAt() const {
    return opensAt_;
}

int WorkingHours::closesAt() const {
    return closesAt_;
}

int WorkingHours::breakStart() const {
    return breakStart_;
}

int WorkingHours::breakEnd() const {
    return breakEnd_;
}

bool WorkingHours::isOpenAt(int minute) const {
    validateMinute(minute);
    return minute >= opensAt_ && minute < closesAt_ &&
           !(minute >= breakStart_ && minute < breakEnd_);
}

int WorkingHours::workingMinutes() const {
    return closesAt_ - opensAt_ - (breakEnd_ - breakStart_);
}

int WorkingHours::nextOpenMinute(int minute) const {
    validateMinute(minute);
    int candidate = minute < opensAt_ ? opensAt_ : minute;
    if(candidate >= breakStart_ && candidate < breakEnd_){
        candidate = breakEnd_;
    }
    if(candidate >= closesAt_){
        throw std::out_of_range("No opening time remains today");
    }
    return candidate;
}

bool WorkingHours::canServe(int startMinute, int durationMinutes) const {
    if(durationMinutes <= 0){
        throw std::invalid_argument("Service duration must be positive");
    }
    if(!isOpenAt(startMinute) || durationMinutes > closesAt_ - startMinute){
        return false;
    }
    if(breakStart_ < breakEnd_ && startMinute < breakStart_ && durationMinutes > breakStart_ - startMinute){
        return false;
    }
    return true;
}
