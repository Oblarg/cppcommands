#include "frc/experimental/command/CommandGroupBase.h"
#include "frc/experimental/command/SequentialCommandGroup.h"
#include "frc/experimental/command/ParallelCommandGroup.h"
#include "frc/errorbase.h"
#include "frc/WPIErrors.h"
#include "frc/experimental/command/ParallelRaceGroup.h"
#include "frc/experimental/command/ParallelDeadlineGroup.h"
#include <set>

using namespace frc::experimental;

template<typename TMap, typename TKey>
static bool ContainsKey(const TMap& map, TKey keyToCheck) {
return map.find(keyToCheck) != map.end();
}

  void CommandGroupBase::RegisterGroupedCommands(wpi::ArrayRef<Command*> commands) {
      GetGroupedCommands().insert(commands.begin(), commands.end());
  }
  void CommandGroupBase::ClearGroupedCommands() {
      GetGroupedCommands().clear();
  }
  void CommandGroupBase::ClearGroupedCommand(Command* command) {
      GetGroupedCommands().erase(command);
  }
  bool CommandGroupBase::RequireUngrouped(wpi::ArrayRef<Command*> commands) {
      bool allUngrouped = true;
      for(auto&& command : commands) {
        allUngrouped &= !ContainsKey(GetGroupedCommands(), command);
      }
      if (!allUngrouped) {
        wpi_setGlobalWPIErrorWithContext(CommandIllegalUse,
            "Commands cannot be added to more than one CommandGroup");
      }
      return allUngrouped;
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
