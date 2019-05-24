#include "SendableCommandBase.h"

namespace frc {
namespace experimental {
class InstantCommand : public SendableCommandBase {
 public:
  InstantCommand(std::function<void()> toRun, wpi::ArrayRef<Subsystem*> requirements) : m_toRun{std::move(toRun)} {
    AddRequirements(requirements);
  }

  InstantCommand() : m_toRun{[]{}} {
  }

  void Initialize() override {
    m_toRun();
  }

  bool IsFinished() final override {
    return true;
  }

 private:
  std::function<void()> m_toRun;
};
}
}
