#pragma once

#include <wpi/Twine.h>
#include <wpi/SmallVector.h>

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
  virtual void GetRequirements(wpi::SmallVectorImpl<Subsystem*> requirements) const = 0;
  Command* WithTimeout(double seconds) const;
  Command* InterruptOn(std::function<bool()> condition) const;
  Command* WhenFinished(std::function<void()> toRun) const;
  Command* BeforeStarting(std::function<void()> toRun) const;
  Command* AndThen(std::initializer_list<Command*> next) const;
  Command* DeadlineWith(std::initializer_list<Command*> parallel) const;
  Command* AlongWith(std::initializer_list<Command*> parallel) const;
  Command* RaceWith(std::initializer_list<Command*> parallel) const;
  Command* Perpetually() const;
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
