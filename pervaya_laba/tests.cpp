#include <UnitTest++/UnitTest++.h>
#include "tape.h"
#include "instruction.h"
#include "program.h"
#include "postmachine.h"
#include <sstream>
TEST(TapeIsEmpty){
    Tape tape;
    CHECK_EQUAL(true, tape.empty());
}
TEST(MarkedCount){
    Tape tape;
    tape.mark(0);
    CHECK_EQUAL(1, tape.count());
    CHECK(tape.isMarked(0));
}

TEST(MarkTwiceCount){
    Tape tape;
    tape.mark(0);
    tape.mark(0);
    CHECK_EQUAL(1, tape.count());
}

TEST(UnmarkUnmarking){
    std::istringstream input("0 V");
    Tape tmptape;
    input >> tmptape;
    Tape tape;
    tape.mark(0);
    tape.mark(1);
    tape.unmark(1);
    CHECK_EQUAL(tmptape, tape);
}

TEST(NegativePos){
    Tape tape;
    tape.mark(-1);
    CHECK_EQUAL(1, tape.count());
}

TEST(minmaxPos){
    Tape tape;
    tape.mark(-2);
    tape.mark(-1);
    tape.mark(4);
    tape.mark(12);
    tape.mark(22);
    tape.mark(16);
    CHECK_EQUAL(-2, tape.minPos());
    CHECK_EQUAL(22, tape.maxPos());

}

TEST(EqualAndNotEqual){
    Tape tape;
    tape.mark(-2);
    tape.mark(3);
    tape.mark(8);
    Tape equaltape;
    equaltape.mark(-2);
    equaltape.mark(3);
    equaltape.mark(8);
    Tape notequaltape;
    notequaltape.mark(-2);
    notequaltape.mark(3);
    notequaltape.mark(5);
    CHECK_EQUAL(true, tape == equaltape);
    CHECK_EQUAL(false, tape != equaltape);
    CHECK_EQUAL(true, tape!= notequaltape);
    CHECK_EQUAL(false, tape == notequaltape);
}

TEST(NotMarkedBetween){
    std::istringstream input("-1 V.V");
    Tape tape;
    input >> tape;
    CHECK_EQUAL(true, tape.isMarked(-1));
    CHECK_EQUAL(false, tape.isMarked(0));
    CHECK_EQUAL(true, tape.isMarked(1));
}
int main(){
    return UnitTest::RunAllTests();
}