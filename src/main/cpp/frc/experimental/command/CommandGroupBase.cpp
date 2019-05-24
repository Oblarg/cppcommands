#include "frc/experimental/command/CommandGroupBase.h"
#include "frc/experimental/command/SequentialCommandGroup.h"
#include "frc/experimental/command/ParallelCommandGroup.h"
#include "frc/experimental/command/ParallelRaceGroup.h"
#include "frc/experimental/command/ParallelDeadlineGroup.h"
#include <set>

using namespace frc::experimental;

  void CommandGroupBase::RegisterGroupedCommands(wpi::ArrayRef<Command*> commands) {
      GetGroupedCommands().insert(commands.begin(), commands.end());
  }
  void CommandGroupBase::ClearGroupedCommands() {
      GetGroupedCommands().clear();
  }
  void CommandGroupBase::ClearGroupedCommand(Command* command) {
      GetGroupedCommands().erase(command);
  }
  void CommandGroupBase::RequireUngrouped(wpi::ArrayRef<Command*> commands) {
      // TODO
  }
  
  std::set<Command*>& CommandGroupBase::GetGroupedCommands() {
    static std::set<Command*> groupedCommands;
    return groupedCommands;
  }

  CommandGroupBase* CommandGroupBase::Sequence(wpi::ArrayRef<Command*> commands) {
      return new SequentialCommandGroup(commands);
  }
  CommandGroupBase* CommandGroupBase::Parallel(wpi::ArrayRef<Command*> commands) {
      return new ParallelCommandGroup(commands);
  }
  CommandGroupBase* CommandGroupBase::Race(wpi::ArrayRef<Command*> commands) {
      return new ParallelRaceGroup(commands);
  }
  CommandGroupBase* CommandGroupBase::Deadline(Command* deadline, wpi::ArrayRef<Command*> commands) {
      return new ParallelDeadlineGroup(deadline, commands);
  }
