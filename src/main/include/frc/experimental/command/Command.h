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
class ParallelCommandGroup;
class ParallelRaceGroup;
class ParallelDeadlineGroup;
class SequentialCommandGroup;
class PerpetualCommand;
class Command {
 public:
  Command() = default;
  Command(Command&& other) = default;
  virtual ~Command() = default;
  virtual void Initialize();
  virtual void Execute();
  virtual void End(bool interrupted);
  virtual bool IsFinished() { return false; }
  virtual wpi::ArrayRef<Subsystem*> GetRequirements() const = 0;

  ParallelRaceGroup WithTimeout(double seconds)&&;
  ParallelRaceGroup InterruptOn(std::function<bool()> condition)&&;
  SequentialCommandGroup BeforeStarting(std::function<void()> toRun)&&;
  SequentialCommandGroup WhenFinished(std::function<void()> toRun)&&;
  template<class... Types, typename = std::enable_if_t<std::conjunction_v<std::is_base_of<Command, Types>...>>>
  SequentialCommandGroup AndThen(Types&&... next)&&;
  template<class... Types, typename = std::enable_if_t<std::conjunction_v<std::is_base_of<Command, Types>...>>>
  ParallelDeadlineGroup DeadlineWith(Types&&... parallel)&&;
  template<class... Types, typename = std::enable_if_t<std::conjunction_v<std::is_base_of<Command, Types>...>>>
  ParallelCommandGroup AlongWith(Types&&... parallel)&&;
  template<class... Types, typename = std::enable_if_t<std::conjunction_v<std::is_base_of<Command, Types>...>>>
  ParallelRaceGroup RaceWith(Types&&... parallel)&&;
  PerpetualCommand Perpetually()&&;
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
  virtual std::unique_ptr<Command> TransferOwnership()&& = 0;

  bool m_isGrouped = false;
};
}
}
