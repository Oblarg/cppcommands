#include "frc/experimental/command/Command.h"
#include "frc/experimental/command/InstantCommand.h"
#include "frc/experimental/command/CommandScheduler.h"

#ifdef __GNUG__
#include <cstdlib>
#include <memory>
#include <cxxabi.h>

namespace frc {
namespace experimental {
std::string demange(const char* name) {
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

void Command::Schedule(bool interruptible) {

}

void Command::Cancel() {

}

bool Command::IsScheduled() const {
  return false;
}

Command* Command::WithTimeout(double seconds) const {
  return new ParallelRaceGroup(this, new WaitCommand(seconds));
}

Command* Command::InterruptOn(std::function<bool()> condition) const {
  return new ParallelRaceGroup(this, new WaitUntilCommand(std::move(condition)));
}

Command* Command::WhenFinished(std::function<void()> toRun) const {
  return new SequentialCommandGroup(this, new InstantCommand(std::move(toRun), {}));
}

Command* Command::BeforeStarting(std::function<void()> toRun) const {
  return new SequentialCommandGroup(new InstantCommand(std::move(toRun), {}), this);
}

Command* Command::AndThen(std::initializer_list<Command*> next) const {
  auto group = new SequentialCommandGroup(this);
  group->AddCommands(next);
  return group;
}

Command* Command::DeadlineWith(std::initializer_list<Command*> parallel) const {
  return new ParallelDeadlineGroup(this, parallel);
}

Command* Command::AlongWith(std::initializer_list<Command*> parallel) const {
  auto group = new ParallelCommandGroup(this);
  group->AddCommands(next);
  return group;
}

Command* Command::RaceWith(std::initializer_list<Command*> parallel) const {
  auto group = new ParallelRaceGroup(this);
  group->AddCommands(next);
  return group;
}

Command* Command::Perpetually() const {
  return new PerpetualCommand(this);
}

void Command::Schedule(bool interruptible) {
  CommandScheduler::GetInstance().ScheduleCommand(this, interruptible);
}

void Command::Cancel() {
  CommandScheduler::GetInstance().Cancel(this);
}

bool Command::IsScheduled() const {
  CommandScheduler::GetInstance().IsScheduled(this);
}

bool Command::HasRequirement(Subsystem* requirement) const {
  return true;
}

std::string Command::GetName() const {
  return GetTypeName(*this);
}


