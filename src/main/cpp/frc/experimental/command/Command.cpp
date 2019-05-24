#include "frc/experimental/command/Command.h"
#include "frc/experimental/command/InstantCommand.h"
#include "frc/experimental/command/CommandScheduler.h"

#include "frc/experimental/command/PerpetualCommand.h"
#include "frc/experimental/command/WaitCommand.h"
#include "frc/experimental/command/ParallelCommandGroup.h"
#include "frc/experimental/command/ParallelRaceGroup.h"
#include "frc/experimental/command/ParallelDeadlineGroup.h"
#include "frc/experimental/command/SequentialCommandGroup.h"
#include "frc/experimental/command/WaitUntilCommand.h"

#ifdef __GNUG__
#include <cstdlib>
#include <memory>
#include <cxxabi.h>

namespace frc {
namespace experimental {
std::string demangle(const char* name) {
    int status = -1;

    std::unique_ptr<char, void(*)(void*)> res {
        abi::__cxa_demangle(name, NULL, NULL, &status),
        std::free
    };

    return (status==0) ? res.get() : name;
}
}
}

#else

namespace frc {
namespace experimental {
std::string demangle(const char* name) {
  return name;
}
}
}

#endif

using namespace frc::experimental;

void Command::Initialize() {}
void Command::Execute() {}
void Command::End(bool interrupted) {}

Command* Command::WithTimeout(double seconds) {
  return new ParallelRaceGroup({this, new WaitCommand(seconds)});
}

Command* Command::InterruptOn(std::function<bool()> condition) {
  return new ParallelRaceGroup({this, new WaitUntilCommand(std::move(condition))});
}

Command* Command::WhenFinished(std::function<void()> toRun) {
  return new SequentialCommandGroup({this, new InstantCommand(std::move(toRun), {})});
}

Command* Command::BeforeStarting(std::function<void()> toRun) {
  return new SequentialCommandGroup({new InstantCommand(std::move(toRun), {}), this});
}

Command* Command::AndThen(wpi::ArrayRef<Command*> next) {
  auto group = new SequentialCommandGroup(this);
  group->AddCommands({next});
  return group;
}

Command* Command::DeadlineWith(wpi::ArrayRef<Command*> parallel) {
  return new ParallelDeadlineGroup({this, parallel});
}

Command* Command::AlongWith(wpi::ArrayRef<Command*> parallel) {
  auto group = new ParallelCommandGroup(this);
  group->AddCommands({parallel});
  return group;
}

Command* Command::RaceWith(wpi::ArrayRef<Command*> parallel) {
  auto group = new ParallelRaceGroup(this);
  group->AddCommands({parallel});
  return group;
}

Command* Command::Perpetually()  {
  return new PerpetualCommand(this);
}

void Command::Schedule(bool interruptible) {
  CommandScheduler::GetInstance().Schedule(interruptible, this);
}

void Command::Cancel() {
  CommandScheduler::GetInstance().Cancel(this);
}

bool Command::IsScheduled() const {
  return CommandScheduler::GetInstance().IsScheduled(this);
}

bool Command::HasRequirement(Subsystem* requirement) const {
  return true;
}

std::string Command::GetName() const {
  return GetTypeName(*this);
}


