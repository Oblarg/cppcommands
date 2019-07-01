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
  // std::unique_ptr<Command> WithTimeout(double seconds)&&;
  // std::unique_ptr<Command> InterruptOn(std::function<bool()> condition)&&;
  // std::unique_ptr<Command> WhenFinished(std::function<void()> toRun)&&;
  // std::unique_ptr<Command> BeforeStarting(std::function<void()> toRun)&&;
  // std::unique_ptr<Command> AndThen(std::vector<std::unique_ptr<Command>>&& next)&&;
  // std::unique_ptr<Command> DeadlineWith(std::vector<std::unique_ptr<Command>>&& parallel)&&;
  // std::unique_ptr<Command> AlongWith(std::vector<std::unique_ptr<Command>>&& parallel)&&;
  // std::unique_ptr<Command> RaceWith(std::vector<std::unique_ptr<Command>>&& parallel)&&;
  // std::unique_ptr<Command> Perpetually()&&;
  void Schedule(bool interruptible);
  void Schedule() { Schedule(true); }
  void Cancel();
  bool IsScheduled() const;
  bool HasRequirement(Subsystem* requirement) const;
  bool IsGrouped() const;
  void SetGrouped(bool grouped);
  virtual bool RunsWhenDisabled() const { return false; }
  virtual std::string GetName() const;
 protected:
  virtual std::unique_ptr<Command> TransferOwnership()&&;
  bool m_isGrouped = false;
};
}
}
