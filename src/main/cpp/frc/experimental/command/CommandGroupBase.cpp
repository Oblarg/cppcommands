#include "frc/experimental/command/CommandGroupBase.h"
#include <unordered_set>

using namespace frc::experimental;

  void CommandGroupBase::RegisterGroupedCommands(std::initializer_list<Command*> commands) {
      auto& grp = GetGroupedCommands();
      for (auto command : commands) {
          grp.emplace(command);
      }
  }
  void CommandGroupBase::ClearGroupedCommands() {
      GetGroupedCommands().clear();
  }
  void CommandGroupBase::ClearGroupedCommand(Command* command) {
      GetGroupedCommands().erase(command);
  }
  void CommandGroupBase::RequireUngrouped(std::initializer_list<Command*> commands) {
      // TODO
  }
  std::unordered_set<Command*>& CommandGroupBase::GetGroupedCommands() {
    static std::unordered_set<Command*> groupedCommands;
    return groupedCommands;
  }

//   CommandGroupBase* CommandGroupBase::Sequence(std::initializer_list<Command*> commands) {

//   }
//   CommandGroupBase* CommandGroupBase::Parallel(std::initializer_list<Command*> commands) {

//   }
//   CommandGroupBase* CommandGroupBase::Race(std::initializer_list<Command*> commands) {

//   }
//   CommandGroupBase* CommandGroupBase::Deadline(std::initializer_list<Command*> commands) {

//   }
