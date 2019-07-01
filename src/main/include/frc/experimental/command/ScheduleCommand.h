#pragma once

#include "SendableCommandBase.h"
#include <wpi/SmallVector.h>
#include "SetUtilities.h"

namespace frc {
namespace experimental {  
class ScheduleCommand : public SendableCommandBase {
 public:
  explicit ScheduleCommand(wpi::ArrayRef<Command*> toSchedule) {
    SetInsert(m_toSchedule, toSchedule);
  }
  
  void Initialize() override {
    for (auto command : m_toSchedule) {
      command->Schedule();
    }
  }
  
  bool IsFinished() override {
    return true;
  }
  
  bool RunsWhenDisabled() const override {
    return true;
  }
 protected:
  std::unique_ptr<Command> TransferOwnership()&& override {
    return std::make_unique<ScheduleCommand>(std::move(*this));
  }
 private:
  wpi::SmallVector<Command*, 4> m_toSchedule;
};
}
}
