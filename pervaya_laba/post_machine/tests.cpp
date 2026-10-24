#include <UnitTest++/UnitTest++.h>
#include "tape.h"
#include "instruction.h"
#include "program.h"
#include "postmachine.h"
#include <sstream>
#include <string>

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

// program tests

TEST(ReadProgram){
    std::istringstream input("V 2\n-> 3\n!\n");
    Program prog;
    input >> prog;
    CHECK_EQUAL(3, prog.size());
    CHECK(!input == false);
}

TEST(HasProgramCorners){
    std::istringstream input("V 2\n-> 3\n!\n");
    Program prog;
    input >> prog;
    CHECK_EQUAL(false, prog.has(0));
    CHECK_EQUAL(false, prog.has(4));
    CHECK_EQUAL(true, prog.has(1));
    CHECK_EQUAL(true, prog.has(2));
    CHECK_EQUAL(true, prog.has(3));

}

TEST(ProgramInsAtPosition){
    std::istringstream input("V 2\n-> 3\n!\n");
    Program prog;
    input >> prog;
    CHECK_EQUAL(Instruction(OpType::Right, 3), prog.at(2));
}

TEST(WorkedInsRemove){
    std::istringstream input("V 2\n-> 3\n!\n");
    Program prog;
    input >> prog;
    CHECK_EQUAL(true, prog.remove(2));
    CHECK_EQUAL(Instruction(),prog.at(2));
    CHECK_EQUAL(2, prog.size());
}

TEST(FailedInsRemove){
    std::istringstream input("V 2\n-> 3\n!\n");
    Program prog;
    input >> prog;
    CHECK_EQUAL(false, prog.remove(5));
    CHECK_EQUAL(3, prog.size());
}

TEST(WrongFormatProgramStreamError){
    std::istringstream input("V 3\n-^ 3\n!\n");
    Program prog;
    input >> prog;
    CHECK_EQUAL(true, !input);
}
// postmachine tests
Program MachineProgram(const std::string &progtext){
    Program prog;
    std::istringstream input(progtext);
    input >> prog;
    return prog;
}

TEST(MachineMarkSetsCellAndJumps){
    PostMachine machine;
    machine.setProgram(MachineProgram("V 2\n!\n"));
    machine.step();
    CHECK_EQUAL(true, machine.tape().isMarked(0));
    CHECK_EQUAL(2, machine.current());
    CHECK(State::Running == machine.state());
}

TEST(MachineMarkOnMarkedCellIsError){
    PostMachine machine;
    Tape tape;
    tape.mark(0);
    machine.setTape(tape);
    machine.setProgram(MachineProgram("V 2\n!\n"));
    machine.step();
    CHECK_EQUAL(true, machine.tape().isMarked(0));
    CHECK_EQUAL(1, machine.current());
    CHECK(State::Error == machine.state());
}

TEST(MachineEraseClearsCell){
    PostMachine machine;
    Tape tape;
    tape.mark(0);
    machine.setTape(tape);
    machine.setProgram(MachineProgram("X 2\n!\n"));
    machine.step();
    CHECK_EQUAL(false, machine.tape().isMarked(0));
    CHECK_EQUAL(2, machine.current());
    CHECK(State::Running == machine.state());
}

TEST(MachineEraseOnEmptyCellIsError){
    PostMachine machine;
    machine.setProgram(MachineProgram("X 2\n!\n"));
    machine.step();
    CHECK(State::Error == machine.state());
    CHECK_EQUAL(1, machine.current());
}

TEST(MachineRightAndLeftMoveCarriage){
    PostMachine machine;
    machine.setProgram(MachineProgram("-> 2\n<- 3\n!\n"));
    machine.step();
    CHECK_EQUAL(1, machine.position());
    machine.step();
    CHECK_EQUAL(0, machine.position());
    CHECK(State::Running == machine.state());
}

TEST(MachineBranchOnMarkedGoesNext1){
    PostMachine machine;
    Tape tape;
    tape.mark(0);
    machine.setTape(tape);
    machine.setProgram(MachineProgram("? 2, 3\n!\n"));
    machine.step();
    CHECK_EQUAL(2, machine.current());
    CHECK(State::Running == machine.state());
}

TEST(MachineBranchOnEmptyGoesNext2){
    PostMachine machine;
    machine.setProgram(MachineProgram("? 2, 3\n!\n"));
    machine.step();
    CHECK_EQUAL(3, machine.current());
    CHECK(State::Running == machine.state());
}

TEST(MachineStopHaltsMachine){
    PostMachine machine;
    machine.setProgram(MachineProgram("!\n"));
    machine.step();
    CHECK_EQUAL(false, machine.step());
    CHECK(State::Stopped == machine.state());
}

TEST(MachineJumpToMissingCommandIsError){
    PostMachine machine;
    machine.setProgram(MachineProgram("V 5\n!\n"));
    CHECK_EQUAL(true, machine.step());
    CHECK_EQUAL(false, machine.step());
    CHECK_EQUAL(true, machine.tape().isMarked(0));
    CHECK(State::Error == machine.state());
}

TEST(MachineRunAddOneProgram){
    PostMachine machine;
    Tape tape;
    tape.mark(0);
    machine.setTape(tape);
    machine.setProgram(MachineProgram("-> 2\n? 1, 3\nV 4\n!\n"));
    CHECK_EQUAL(4, machine.run(100));
    CHECK_EQUAL(true, machine.tape().isMarked(0));
    CHECK_EQUAL(true, machine.tape().isMarked(1));
    CHECK_EQUAL(4, machine.current());
    CHECK(State::Stopped == machine.state());

}

TEST(MachineIncrementOperators){
    PostMachine machine;
    machine.setProgram(MachineProgram("-> 2\n? 1, 3\nV 4\n!\n"));
    PostMachine plusm = machine;
    PostMachine mplus = machine;
    PostMachine plusmachine = ++plusm;
    CHECK_EQUAL(2, plusm.current());
    CHECK_EQUAL(2, plusmachine.current());
    PostMachine machineplus = mplus++;
    CHECK_EQUAL(2, mplus.current());
    CHECK_EQUAL(1, machineplus.current());
}



int main(){
    return UnitTest::RunAllTests();
}