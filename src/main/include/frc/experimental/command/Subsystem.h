#pragma once

namespace frc {
namespace experimental {
class Command;
class Subsystem {
 public:
  ~Subsystem() = default;
  virtual void Periodic();
  virtual void SetDefaultCommand(Command* command);
  virtual Command* GetDefaultCommand() const;
  virtual Command* GetCurrentCommand() const;
  virtual void Register() const;
};
}
}
