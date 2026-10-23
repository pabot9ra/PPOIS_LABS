#include <UnitTest++/UnitTest++.h>
#include "tape.h"
#include "instruction.h"
#include "program.h"
#include "postmachine.h"
#include <sstream>

// tape tests
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
    Tape tape;
    tape.mark(0);
    tape.mark(1);
    tape.unmark(1);
    CHECK_EQUAL(false, tape.isMarked(1));
    CHECK_EQUAL(true, tape.isMarked(0));
    CHECK_EQUAL(1, tape.count());
}

TEST(NegativePos){
    Tape tape;
    tape.mark(-1);
    CHECK_EQUAL(true, tape.isMarked(-1));
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
// instruction tests
TEST(InsTypeAndNext1){
    Instruction ins(OpType::Mark, 1);
    CHECK(ins.type() == OpType::Mark);
    CHECK_EQUAL(1, ins.next1());
}

TEST(InsBranchConstructor){
    Instruction ins(OpType::Branch, 3, 5);
    CHECK(OpType::Branch == ins.type());
    CHECK_EQUAL(3, ins.next1());
    CHECK_EQUAL(5, ins.next2());
}
TEST(InsDefaultConstructor){
    Instruction ins;
    CHECK(OpType::Stop == ins.type());
    CHECK_EQUAL(0, ins.next1());
    CHECK_EQUAL(0, ins.next2());
}

TEST(InsStream){
    std::istringstream input("? 3, 1");
    Instruction ins;
    input >> ins;
    CHECK(OpType::Branch == ins.type());
    CHECK_EQUAL(3, ins.next1());
    CHECK_EQUAL(1, ins.next2());
    CHECK_EQUAL(false, !input);
}

TEST(InsStreamError){
    std::istringstream input("L 5");
    Instruction ins;
    input >> ins;
    CHECK_EQUAL(true, !input);
}
int main(){
    return UnitTest::RunAllTests();
}