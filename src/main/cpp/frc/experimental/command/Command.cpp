#include "frc/experimental/command/Command.h"
#include "frc/experimental/command/InstantCommand.h"
#include "frc/experimental/command/CommandScheduler.h"

#include "frc/experimental/command/PerpetualCommand.h"
#include "frc/experimental/command/WaitCommand.h"
#include "frc/experimental/command/ParallelRaceGroup.h"
#include "frc/experimental/command/ParallelCommandGroup.h"
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

ParallelRaceGroup Command::WithTimeout(double seconds)&& {
  std::vector<std::unique_ptr<Command>> temp;
  temp.emplace_back(std::make_unique<WaitCommand>(seconds));
  temp.emplace_back(std::move(*this).TransferOwnership());
  return ParallelRaceGroup(std::move(temp));
}

ParallelRaceGroup Command::InterruptOn(std::function<bool()> condition)&& {
  std::vector<std::unique_ptr<Command>> temp;
  temp.emplace_back(std::make_unique<WaitUntilCommand>(std::move(condition)));
  temp.emplace_back(std::move(*this).TransferOwnership());
  return ParallelRaceGroup(std::move(temp));
}

SequentialCommandGroup Command::WhenFinished(std::function<void()> toRun)&& {
  std::vector<std::unique_ptr<Command>> temp;
  temp.emplace_back(std::make_unique<InstantCommand>(std::move(toRun)));
  temp.emplace_back(std::move(*this).TransferOwnership());
  return SequentialCommandGroup(std::move(temp));
}

SequentialCommandGroup Command::BeforeStarting(std::function<void()> toRun)&& {
  std::vector<std::unique_ptr<Command>> temp;
  temp.emplace_back(std::move(*this).TransferOwnership());
  temp.emplace_back(std::make_unique<InstantCommand>(std::move(toRun)));
  return SequentialCommandGroup(std::move(temp));
}

template<class... Types, typename>
SequentialCommandGroup Command::AndThen(Types&&... next)&& {
  std::vector<std::unique_ptr<Command>> temp;
  temp.emplace_back(std::move(*this).TransferOwnership());
  ((void)temp.emplace_back(std::make_unique<Types>(std::forward<Types>(next))), ...);
  return SequentialCommandGroup(std::move(temp));
}

template<class... Types, typename>
ParallelDeadlineGroup Command::DeadlineWith(Types&&... parallel)&& {
  std::vector<std::unique_ptr<Command>> temp;
  ((void)temp.emplace_back(std::make_unique<Types>(std::forward<Types>(next))), ...);
  return ParallelDeadlineGroup(std::move(*this).TransferOwnership(), temp);
}

template<class... Types, typename>
ParallelCommandGroup Command::AlongWith(Types&&... next)&& {
  std::vector<std::unique_ptr<Command>> temp;
  temp.emplace_back(std::move(*this).TransferOwnership());
  ((void)temp.emplace_back(std::make_unique<Types>(std::forward<Types>(next))), ...);
  return ParallelCommandGroup(std::move(temp));
}

template<class... Types, typename>
ParallelRaceGroup Command::RaceWith(Types&&... next)&& {
  std::vector<std::unique_ptr<Command>> temp;
  temp.emplace_back(std::move(*this).TransferOwnership());
  ((void)temp.emplace_back(std::make_unique<Types>(std::forward<Types>(next))), ...);
  return ParallelRaceGroup(std::move(temp));
}

PerpetualCommand Command::Perpetually()&&  {
  return PerpetualCommand(std::move(*this).TransferOwnership());
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


