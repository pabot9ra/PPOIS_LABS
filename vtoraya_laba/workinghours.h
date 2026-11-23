#ifndef WORKINGHOURS_H
#define WORKINGHOURS_H

class WorkingHours {
private:
    int opensAt_;
    int closesAt_;
    int breakStart_;
    int breakEnd_;

    static void validateMinute(int minute);

public:
    static const int MINUTES_PER_DAY = 24 * 60;

    WorkingHours(int opensAt, int closesAt, int breakStart, int breakEnd);

    int opensAt() const;
    int closesAt() const;
    int breakStart() const;
    int breakEnd() const;
    bool isOpenAt(int minute) const;
    int workingMinutes() const;
    int nextOpenMinute(int minute) const;
    bool canServe(int startMinute, int durationMinutes) const;
};

#endif
