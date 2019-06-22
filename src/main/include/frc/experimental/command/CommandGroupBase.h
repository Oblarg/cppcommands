#pragma once

#include "SendableCommandBase.h"
#include <set>

namespace frc {
namespace experimental {
class CommandGroupBase : public SendableCommandBase {
 public:
  static void RegisterGroupedCommands(wpi::ArrayRef<Command*> commands);
  static void ClearGroupedCommands();
  static void ClearGroupedCommand(Command* command);
  static bool RequireUngrouped(wpi::ArrayRef<Command*> commands);
  static std::set<Command*>& GetGroupedCommands();

  static CommandGroupBase* Sequence(wpi::ArrayRef<Command*> commands);
  static CommandGroupBase* Parallel(wpi::ArrayRef<Command*> commands);
  static CommandGroupBase* Race(wpi::ArrayRef<Command*> commands);
  static CommandGroupBase* Deadline(Command* deadline, wpi::ArrayRef<Command*> commands);

  virtual void AddCommands(wpi::ArrayRef<Command*> commands) = 0;
};
}
}
