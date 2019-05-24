#pragma once

#include <wpi/Twine.h>
#include <wpi/ArrayRef.h>

namespace frc {
namespace experimental {
std::string demangle(const char* name);

template <typename T>
std::string GetTypeName(const T& type) {
  return demangle(typeid(type).name());
}

class Subsystem;
class Command {
 public:
  virtual ~Command() = default;
  virtual void Initialize();
  virtual void Execute();
  virtual void End(bool interrupted);
  virtual bool IsFinished() { return false; }
  virtual wpi::ArrayRef<Subsystem*> GetRequirements() const = 0;
  Command* WithTimeout(double seconds);
  Command* InterruptOn(std::function<bool()> condition);
  Command* WhenFinished(std::function<void()> toRun);
  Command* BeforeStarting(std::function<void()> toRun);
  Command* AndThen(wpi::ArrayRef<Command*> next);
  Command* DeadlineWith(wpi::ArrayRef<Command*> parallel);
  Command* AlongWith(wpi::ArrayRef<Command*> parallel);
  Command* RaceWith(wpi::ArrayRef<Command*> parallel);
  Command* Perpetually();
  void Schedule(bool interruptible);
  void Schedule() { Schedule(true); }
  void Cancel();
  bool IsScheduled() const;
  bool HasRequirement(Subsystem* requirement) const;
  virtual bool RunsWhenDisabled() { return false; }
  virtual std::string GetName() const;
};
}
}
