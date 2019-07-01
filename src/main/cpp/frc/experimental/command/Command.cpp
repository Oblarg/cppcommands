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

std::unique_ptr<Command> Command::WithTimeout(double seconds)&& {
  return std::make_unique<ParallelRaceGroup>(
    std::vector<std::unique_ptr<Command>>{
      std::move(*this).TransferOwnership(), 
      std::make_unique<WaitCommand>(seconds)});
}

std::unique_ptr<Command> Command::InterruptOn(std::function<bool()> condition)&& {
  return std::make_unique<ParallelRaceGroup>(
    std::vector<std::unique_ptr<Command>>{
      std::move(*this).TransferOwnership(), 
      std::make_unique<WaitUntilCommand>(condition)});
}

std::unique_ptr<Command> Command::WhenFinished(std::function<void()> toRun)&& {
  return std::make_unique<SequentialCommandGroup>(
    std::vector<std::unique_ptr<Command>>{
      std::move(*this).TransferOwnership(), 
      std::make_unique<InstantCommand>(toRun)});
}

std::unique_ptr<Command> Command::BeforeStarting(std::function<void()> toRun)&& {
  return std::make_unique<SequentialCommandGroup>(
    std::vector<std::unique_ptr<Command>>{
      std::make_unique<InstantCommand>(toRun)}),
      std::move(*this).TransferOwnership();
}

std::unique_ptr<Command> Command::AndThen(std::vector<std::unique_ptr<Command>>&& next)&& {
  next.insert(next.begin(), std::move(*this).TransferOwnership());
  return std::make_unique<SequentialCommandGroup>(next);
}

std::unique_ptr<Command> Command::DeadlineWith(std::vector<std::unique_ptr<Command>>&& next)&& {
  return std::make_unique<SequentialCommandGroup>(std::move(*this).TransferOwnership(), next);
}

std::unique_ptr<Command> Command::AlongWith(std::vector<std::unique_ptr<Command>>&& next)&& {
  next.insert(next.begin(), std::move(*this).TransferOwnership());
  return std::make_unique<ParallelCommandGroup>(next);
}

std::unique_ptr<Command> Command::RaceWith(std::vector<std::unique_ptr<Command>>&& next)&& {
  next.insert(next.begin(), std::move(*this).TransferOwnership());
  return std::make_unique<ParallelRaceGroup>(next);
}

std::unique_ptr<Command> Command::Perpetually()&&  {
  return std::make_unique<PerpetualCommand>(std::move(*this).TransferOwnership());
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
  bool hasRequirement = false;
  for (auto&& subsystem: GetRequirements()) {
    hasRequirement |= requirement == subsystem;
  }
  return hasRequirement;
}

std::string Command::GetName() const {
  return GetTypeName(*this);
}

bool Command::IsGrouped() const {
  return m_isGrouped;
}

void Command::SetGrouped(bool grouped) {
  m_isGrouped = grouped;
}
