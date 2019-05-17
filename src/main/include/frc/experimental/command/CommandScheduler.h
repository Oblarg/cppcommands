#pragma once

#include <frc/smartdashboard/SendableBase.h>
#include <wpi/DenseMap.h>
#include <wpi/SmallSet.h>
#include <networktables/NetworkTableEntry.h>
#include <frc/RobotState.h>
#include "CommandState.h"

namespace frc {
namespace experimental {
class Command;
class Subsystem;
class CommandScheduler final : public SendableBase {
 public:
  static CommandScheduler& GetInstance();
  using Action = std::function<void(const Command&)>;

  void AddButton(std::function<void()>&& button) {
    m_buttons.emplace_back(std::move(button));
  }

  void ClearButtons() {
    m_buttons.clear();
  }

  void ScheduleCommand(Command* command, bool interruptible);

  void ScheduleCommands(bool interruptible, std::initializer_list<Command*> commands) {
    for (auto&& command : commands) {
      ScheduleCommand(command, interruptible);
    }
  }

  void Run();

  void RegisterSubsystem(std::initializer_list<Subsystem*> subsystems);

  void UnregisterSubsystem(std::initializer_list<Subsystem*> subsystems);

  void SetDefaultCommand(Subsystem* subsystem, Command* defaultCommand);

  Command* GetDefaultCommand(Subsystem* subsystem) const;

  void CancelCommands(std::initializer_list<Command*> commands);
  void CancelAll();

  double TimeSinceScheduled(Command* command) const ;
  bool IsScheduled(std::initializer_list<Command*> commands) const;

  void Disable() { m_disabled = true; }
  void Enable() { m_disabled = false; }

  void OnCommandInitialize(Action action) {
    m_initActions.emplace_back(std::move(action));
  }
  void OnCommandExecute(Action action) {
    m_executeActions.emplace_back(std::move(action));
  }
  void OnCommandInterrupt(Action action) {
    m_interruptActions.emplace_back(std::move(action));
  }
  void OnCommandFinish(Action action) {
    m_finishActions.emplace_back(std::move(action));
  }

  void InitSendable(SendableBuilder& builder) override;

 private:
  CommandScheduler();

  wpi::DenseMap<Command*, CommandState> m_scheduledCommands;
  wpi::DenseMap<Subsystem*, Command*> m_requirements;
  wpi::DenseMap<Subsystem*, Command*> m_subsystems;
  wpi::SmallVector<std::function<void()>, 4> m_buttons;

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
