#pragma once

#include "SendableCommandBase.h"
#include <wpi/SmallVector.h>
#include "SetUtilities.h"

namespace frc {
namespace experimental {
class BlockingScheduleCommand : public SendableCommandBase {
 public:
  BlockingScheduleCommand(wpi::ArrayRef<Command*> toSchedule) {
    SetInsert(m_toSchedule, toSchedule);
  }

  void Initialize() override {
    m_finished = false;
    for (auto* command : m_toSchedule) {
      command->Schedule();
    }
  }

  void End(bool interrupted) override {
    if (interrupted) {
      for (auto* command : m_toSchedule) {
        command->Cancel();
      }
    }
  }

  void Execute() override {
    for (auto* command : m_toSchedule) {
      m_finished &= !command->IsScheduled();
    }
  }

  bool IsFinished() override {
    return m_finished;
  }
 private:
  wpi::SmallVector<Command*,4> m_toSchedule;
  bool m_finished{false};
};
}
}
