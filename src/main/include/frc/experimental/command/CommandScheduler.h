#pragma once

#include <frc/smartdashboard/SendableBase.h>
#include <wpi/DenseMap.h>
#include <wpi/SmallSet.h>
#include <networktables/NetworkTableEntry.h>
#include <frc/RobotState.h>

namespace frc {
namespace experimental {
class Command;
class Subsystem;
class ButtonScheduler;
struct CommandState {

};
class CommandScheduler final : SendableBase {
 public:
  static CommandScheduler& GetInstance();
  using Action = std::function<void(Command&)>;

  void AddButton(std::unique_ptr<ButtonScheduler> button) {
    m_buttons.emplace_back(std::move(button));
  }

  void ClearButtons() {
    m_buttons.clear();
  }

  void ScheduleCommand(Command* command, bool interruptible) {
  }

  template<typename... Args>
  void ScheduleCommands(bool interruptible, Command* command, Args... arg) {
    ScheduleCommand(interruptible, command);
    ScheduleCommands(interruptible, arg);
  }

  void Run();

  template<typename... Args>
  void RegisterSubsystem(Subsystem* subsystem, Args... arg) {
    m_subsystems[subsystem] = nullptr;
    RegisterSubsystem(arg);
  }

  template<typename... Args>
  void UnregisterSubsystem(Subsystem* subsystem, Args... arg) {
    auto s = m_subsystems.find(subsystem);
    if (s != m_subsystems.end()) {
      m_subsystems.erase(s);
    }
    UnregisterSubsystem(arg);
  }

  void SetDefaultCommand(Subsystem* subsystem, Command* defaultCommand) {
    m_subsystems[subsystem] = defaultCommand;
  }

  Command* GetDefaultCommand(Subsystem* subsystem) {
    auto find = m_subsystems.find(subsystem);
    if (find != m_subsystems.end()) {
      return find->second;
    }
    return nullptr;
  }

 private:
  CommandScheduler();

  wpi::DenseMap<Command*, CommandState> m_scheduledCommands;
  wpi::DenseMap<Subsystem*, Command*> m_requirements;
  wpi::DenseMap<Subsystem*, Command*> m_subsystems;
  wpi::SmallVector<std::unique_ptr<ButtonScheduler>, 4> m_buttons;

  bool m_disabled{false};

  nt::NetworkTableEntry m_namesEntry;
  nt::NetworkTableEntry m_idsEntry;
  nt::NetworkTableEntry m_cancelEntry;

  wpi::SmallVector<Action, 4> m_initActions;
  wpi::SmallVector<Action, 4> m_executeActions;
  wpi::SmallVector<Action, 4> m_interruptActions;
  wpi::SmallVector<Action, 4> m_finishActions;

};
}
}
