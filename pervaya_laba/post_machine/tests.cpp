#include <UnitTest++/UnitTest++.h>
#include "tape.h"
#include "instruction.h"
#include "program.h"
#include "postmachine.h"
#include <sstream>
#include <string>
#include <stdexcept>

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





TEST(TapeEmptyBoundsAndClear){
    Tape tape;
    CHECK_EQUAL(0, tape.minPos());
    CHECK_EQUAL(0, tape.maxPos());
    tape.mark(-3);
    tape.mark(5);
    tape.clear();
    CHECK(tape.empty());
    CHECK_EQUAL(0, tape.count());
    CHECK(!tape.isMarked(-3));
    CHECK(!tape.isMarked(5));
}

TEST(TapeUnmarkMissingCellDoesNotChangeOtherMarks){
    Tape tape;
    tape.mark(-4);
    tape.unmark(9);
    CHECK_EQUAL(1, tape.count());
    CHECK(tape.isMarked(-4));
}

TEST(TapeInputReplacesPreviousContents){
    Tape tape;
    tape.mark(100);
    std::istringstream input("-2 V..VV");
    input >> tape;
    CHECK(!input.fail());
    CHECK_EQUAL(3, tape.count());
    CHECK(tape.isMarked(-2));
    CHECK(tape.isMarked(1));
    CHECK(tape.isMarked(2));
    CHECK(!tape.isMarked(-1));
    CHECK(!tape.isMarked(0));
    CHECK(!tape.isMarked(100));
}

TEST(TapeAllDotsProducesEmptyTape){
    Tape tape;
    tape.mark(4);
    std::istringstream input("-3 ....");
    input >> tape;
    CHECK(!input.fail());
    CHECK(tape.empty());
}

TEST(TapeRejectsInvalidPositionAndMissingContents){
    const char* inputs[] = {"invalid V", "", "3"};
    for (const char* text : inputs){
        Tape tape;
        tape.mark(8);
        std::istringstream input(text);
        input >> tape;
        CHECK(input.fail());
        CHECK_EQUAL(1, tape.count());
        CHECK(tape.isMarked(8));
    }
}

TEST(TapeRejectsUnknownCellSymbol){
    Tape tape;
    std::istringstream input("0 V?V");
    input >> tape;
    CHECK(input.fail());
    // При ошибке символа интерфейс Tape не обещает сохранение старой ленты.
}

TEST(TapeOutputShowsEmptyAndSparseTape){
    Tape tape;
    std::ostringstream emptyOutput;
    emptyOutput << tape;
    CHECK_EQUAL(std::string("(empty tape)"), emptyOutput.str());
    tape.mark(-1);
    tape.mark(1);
    std::ostringstream markedOutput;
    markedOutput << tape;
    CHECK_EQUAL(std::string("[-1] V . V "), markedOutput.str());
}

TEST(TapeCopyAndAssignmentAreIndependent){
    Tape original;
    original.mark(-1);
    Tape copied(original);
    Tape assigned;
    assigned = original;
    copied.mark(2);
    assigned.unmark(-1);
    CHECK_EQUAL(1, original.count());
    CHECK(original.isMarked(-1));
    CHECK(!original.isMarked(2));
    CHECK_EQUAL(2, copied.count());
    CHECK(assigned.empty());
}

TEST(AllInstructionTypesRoundTrip){
    const Instruction instructions[] = {
        Instruction(OpType::Mark, 2), Instruction(OpType::Erase, 3),
        Instruction(OpType::Right, 4), Instruction(OpType::Left, 5),
        Instruction(OpType::Branch, 6, 7), Instruction()
    };
    const char* expected[] = {"V 2", "X 3", "-> 4", "<- 5", "? 6, 7", "!"};
    for (int index = 0; index < 6; ++index){
        std::ostringstream output;
        output << instructions[index];
        CHECK_EQUAL(std::string(expected[index]), output.str());
        Instruction restored(OpType::Mark, 99);
        std::istringstream input(output.str());
        input >> restored;
        CHECK(!input.fail());
        CHECK(restored == instructions[index]);
        CHECK(!(restored != instructions[index]));
    }
}

TEST(InstructionEqualityComparesTypeAndBothTargets){
    const Instruction branch(OpType::Branch, 2, 3);
    CHECK(branch != Instruction(OpType::Branch, 4, 3));
    CHECK(branch != Instruction(OpType::Branch, 2, 4));
    CHECK(Instruction(OpType::Left, 2) != Instruction(OpType::Right, 2));
    CHECK_EQUAL(0, Instruction(OpType::Mark, 2).next2());
}

TEST(UnknownInstructionLeavesExistingInstructionUnchanged){
    const Instruction expected(OpType::Branch, 2, 3);
    Instruction instruction = expected;
    std::istringstream input("unknown 4");
    input >> instruction;
    CHECK(input.fail());
    CHECK(instruction == expected);
}

TEST(ProgramDefaultClearAndEmptyOutput){
    Program program;
    CHECK(program.empty());
    CHECK_EQUAL(0, program.size());
    CHECK(!program.has(-1));
    CHECK(!program.has(1));
    program.add(Instruction());
    program.clear();
    CHECK(program.empty());
    std::ostringstream output;
    output << program;
    CHECK(output.str().empty());
}

TEST(ProgramAtRejectsOutOfRangeNumbers){
    Program program;
    CHECK_THROW(program.at(1), std::out_of_range);
    program.add(Instruction());
    CHECK_THROW(program.at(0), std::out_of_range);
    CHECK_THROW(program.at(-1), std::out_of_range);
    CHECK_THROW(program.at(2), std::out_of_range);
}

TEST(ProgramRemovalShiftsCommandsWithoutRewritingTargets){
    Program program = MachineProgram("V 3\n-> 3\n!\n");
    CHECK(!program.remove(0));
    CHECK(!program.remove(-1));
    CHECK_EQUAL(3, program.size());
    CHECK(program.remove(2));
    CHECK_EQUAL(2, program.size());
    CHECK_EQUAL(3, program.at(1).next1());
    CHECK(program.at(2).type() == OpType::Stop);
    CHECK(program.remove(2));
    CHECK(program.remove(1));
    CHECK(program.empty());
    CHECK(!program.remove(1));
}

TEST(ProgramRoundTripPreservesCommandOrder){
    const Program original = MachineProgram("V 2\nX 3\n-> 4\n<- 5\n? 1, 6\n!\n");
    std::ostringstream output;
    output << original;
    CHECK_EQUAL(std::string("V 2\nX 3\n-> 4\n<- 5\n? 1, 6\n!\n"), output.str());
    Program restored;
    std::istringstream input(output.str());
    input >> restored;
    CHECK(!input.fail());
    CHECK(restored == original);
    CHECK(!(restored != original));
}

TEST(ProgramReadReplacesPreviousCommands){
    Program program = MachineProgram("V 2\n!\n");
    std::istringstream input("<- 2\n!\n");
    input >> program;
    CHECK(!input.fail());
    CHECK_EQUAL(2, program.size());
    CHECK(program.at(1) == Instruction(OpType::Left, 2));
}

TEST(ProgramInvalidCommandKeepsOnlySuccessfullyReadPrefix){
    Program program = MachineProgram("!\n");
    std::istringstream input("V 2\nunknown\n!\n");
    input >> program;
    CHECK(input.fail());
    CHECK_EQUAL(1, program.size());
    CHECK(program.at(1) == Instruction(OpType::Mark, 2));
}

TEST(ProgramCopiesDoNotShareCommandStorage){
    const Program original = MachineProgram("V 2\n!\n");
    Program copied(original);
    Program assigned;
    assigned = original;
    copied.remove(1);
    assigned.clear();
    CHECK_EQUAL(2, original.size());
    CHECK_EQUAL(1, copied.size());
    CHECK(assigned.empty());
    CHECK(copied != original);
}

TEST(MachineConstructorCopiesInputTapeAndProgram){
    Tape tape;
    tape.mark(-3);
    Program program = MachineProgram("!\n");
    PostMachine machine(tape, program, -3);
    tape.clear();
    program.clear();
    CHECK(machine.tape().isMarked(-3));
    CHECK_EQUAL(1, machine.program().size());
    CHECK_EQUAL(-3, machine.position());
    CHECK_EQUAL(1, machine.current());
    CHECK(machine.state() == State::Running);
}

TEST(MachineResetPreservesTapeAndCarriage){
    PostMachine machine;
    machine.setPosition(-2);
    machine.setProgram(MachineProgram("V 2\n!\n"));
    CHECK_EQUAL(2, machine.run(10));
    machine.reset();
    CHECK(machine.state() == State::Running);
    CHECK_EQUAL(1, machine.current());
    CHECK_EQUAL(-2, machine.position());
    CHECK(machine.tape().isMarked(-2));
}

TEST(MachineRunLimitAllowsResumingExecution){
    PostMachine machine;
    machine.setProgram(MachineProgram("-> 1\n"));
    CHECK_EQUAL(0, machine.run(0));
    CHECK_EQUAL(0, machine.run(-1));
    CHECK_EQUAL(0, machine.position());
    CHECK_EQUAL(3, machine.run(3));
    CHECK_EQUAL(3, machine.position());
    CHECK(machine.state() == State::Running);
    CHECK_EQUAL(2, machine.run(2));
    CHECK_EQUAL(5, machine.position());
}

TEST(MachineFailedInstructionIsNotCountedAsExecuted){
    PostMachine machine;
    machine.setProgram(MachineProgram("X 1\n"));
    CHECK_EQUAL(0, machine.run(10));
    CHECK(machine.state() == State::Error);
    const PostMachine failed = machine;
    CHECK(!machine.step());
    CHECK_EQUAL(0, machine.run(10));
    CHECK(machine == failed);
}

TEST(MachineEmptyProgramFailsWithoutMovingCarriage){
    PostMachine machine;
    CHECK(!machine.step());
    CHECK(machine.state() == State::Error);
    CHECK_EQUAL(0, machine.position());
    CHECK_EQUAL(1, machine.current());
    CHECK(machine.tape().empty());
}

TEST(MachineCanRestartAfterReplacingFailedProgram){
    PostMachine machine;
    CHECK(!machine.step());
    machine.setProgram(MachineProgram("!\n"));
    CHECK(machine.state() == State::Error);
    CHECK(!machine.step());
    machine.reset();
    CHECK(machine.step());
    CHECK(machine.state() == State::Stopped);
}

TEST(MachinePrefixReturnsReferenceAndPostfixReturnsIndependentSnapshot){
    PostMachine machine;
    machine.setProgram(MachineProgram("V 2\n-> 3\n!\n"));
    const PostMachine before = machine;
    const PostMachine snapshot = machine++;
    CHECK(snapshot == before);
    CHECK(snapshot.tape().empty());
    CHECK(machine.tape().isMarked(0));
    PostMachine& reference = ++machine;
    CHECK(&reference == &machine);
    CHECK_EQUAL(1, machine.position());
    CHECK_EQUAL(0, snapshot.position());
    CHECK_EQUAL(3, machine.current());
}

TEST(MachineEqualityDistinguishesEachStateComponent){
    const Program program = MachineProgram("V 1\n");
    Tape tape;
    tape.mark(0);
    const PostMachine original(tape, program, 0);
    PostMachine changed = original;
    changed.setTape(Tape());
    CHECK(changed != original);
    changed = original;
    changed.setProgram(MachineProgram("!\n"));
    CHECK(changed != original);
    changed = original;
    changed.setPosition(1);
    CHECK(changed != original);
    changed = original;
    CHECK(!changed.step()); // Только состояние меняется: V на помеченной ячейке.
    CHECK(changed != original);
    PostMachine running;
    running.setProgram(MachineProgram("? 2, 2\n!\n"));
    changed = running;
    CHECK(changed.step()); // Только номер текущей команды меняется.
    CHECK(changed != running);
    CHECK(original == original);
}

TEST(MachineReadsCompleteFileAndResetsExecutionState){
    PostMachine machine;
    machine.setProgram(MachineProgram("!\n"));
    machine.step();
    std::istringstream input("tape: -2 V.V\ncarriage: -1\nV 2\n!\n");
    input >> machine;
    CHECK(!input.fail());
    CHECK(machine.state() == State::Running);
    CHECK_EQUAL(1, machine.current());
    CHECK_EQUAL(-1, machine.position());
    CHECK_EQUAL(2, machine.tape().count());
    CHECK(machine.tape().isMarked(-2));
    CHECK(machine.tape().isMarked(0));
    CHECK_EQUAL(2, machine.program().size());
    CHECK_EQUAL(2, machine.run(10));
    CHECK(machine.tape().isMarked(-1));
    CHECK(machine.state() == State::Stopped);
}

TEST(MachineMalformedFileDoesNotChangeExistingMachine){
    const char* inputs[] = {
        "", "wrong: 0 V\ncarriage: 0\n!\n",
        "tape: bad V\ncarriage: 0\n!\n",
        "tape: 0 V?\ncarriage: 0\n!\n",
        "tape: 0 V\nwrong: 0\n!\n",
        "tape: 0 V\ncarriage: bad\n!\n",
        "tape: 0 V\ncarriage: 0\nunknown\n"
    };
    Tape tape;
    tape.mark(9);
    PostMachine machine(tape, MachineProgram("-> 2\n!\n"), 9);
    machine.step();
    const PostMachine original = machine;
    for (const char* text : inputs){
        std::istringstream input(text);
        input >> machine;
        CHECK(input.fail());
        CHECK(machine == original);
    }
}

TEST(MachineOutputShowsRunningStoppedAndErrorStates){
    PostMachine machine;
    machine.setProgram(MachineProgram("!\n"));
    std::ostringstream running;
    running << machine;
    CHECK_EQUAL(std::string("[Running]\ncommand: 1 !\ncarriage: 0\ntape: (empty tape)\n"), running.str());
    machine.step();
    std::ostringstream stopped;
    stopped << machine;
    CHECK(stopped.str().find("[Stopped]\n") == 0);
    PostMachine failed;
    failed.step();
    std::ostringstream error;
    error << failed;
    CHECK(error.str().find("[Error]\n") == 0);
    CHECK(error.str().find("no such command with number: 1\n") != std::string::npos);
}

int main(){
    return UnitTest::RunAllTests();
}