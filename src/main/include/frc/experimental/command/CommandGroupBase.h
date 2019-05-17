#pragma once

#include "SendableCommandBase.h"
#include <unordered_set>

namespace frc {
namespace experimental {
class CommandGroupBase : public SendableCommandBase {
 public:
  static void RegisterGroupedCommands(std::initializer_list<Command*> commands);
  static void ClearGroupedCommands();
  static void ClearGroupedCommand(Command* command);
  static void RequireUngrouped(std::initializer_list<Command*> commands);
  static std::unordered_set<Command*>& GetGroupedCommands();

  static CommandGroupBase* Sequence(std::initializer_list<Command*> commands);
  static CommandGroupBase* Parallel(std::initializer_list<Command*> commands);
  static CommandGroupBase* Race(std::initializer_list<Command*> commands);
  static CommandGroupBase* Deadline(std::initializer_list<Command*> commands);

  virtual void AddCommands(std::initializer_list<Command*> commands) = 0;
};
}
}
