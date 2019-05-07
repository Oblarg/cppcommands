#pragma once

#include <wpi/Twine.h>
#include <wpi/SmallVector.h>


namespace frc {
namespace experimental {
class Subsystem;
class Command {
 public:
  Command(const wpi::Twine& name);
  virtual ~Command() = default;
  virtual void Initialize() {}
  virtual void Execute() {}
  virtual void End(bool interrupted) {}
  virtual bool IsFinished() { return false; }
  virtual void GetRequirements(wpi::SmallVectorImpl<Subsystem*> requirements) = 0;
};
}
}
