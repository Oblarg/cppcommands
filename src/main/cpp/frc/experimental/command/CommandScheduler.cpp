#include <frc/experimental/command/CommandScheduler.h>
#include <frc/experimental/command/Subsystem.h>
#include <frc/experimental/command/CommandGroupBase.h>
#include <frc/commands/Scheduler.h>
#include <hal/HAL.h>

using namespace frc::experimental;

CommandScheduler::CommandScheduler() {
  SetName("Scheduler");
}

CommandScheduler& CommandScheduler::GetInstance() {
  static CommandScheduler scheduler;
  return scheduler;
}

void CommandScheduler::ScheduleCommand(Command* command, bool interruptible) {
  auto& groupedCommands = CommandGroupBase::GetGroupedCommands();
  if (groupedCommands.find(command) != groupedCommands.end()) {
    // TODO: Illegal use of command
  }
  if (m_disabled || (RobotState::IsDisabled() && !command->RunsWhenDisabled())) {
    return;
  }

  wpi::SmallVector<Subsystem*, 8> requirements;
  command->GetRequirements(requirements);

  bool noRequirementsInUse = false;
}

void CommandScheduler::Run() {

}

void CommandScheduler::RegisterSubsystem(std::initializer_list<Subsystem*> subsystems) {
  for (auto* subsystem : subsystems) {
      m_subsystems[subsystem] = nullptr;
    }
}

void CommandScheduler::UnregisterSubsystem(std::initializer_list<Subsystem*> subsystems) {
    for (auto* subsystem : subsystems) {
      auto s = m_subsystems.find(subsystem);
      if (s != m_subsystems.end()) {
        m_subsystems.erase(s);
      }
    }
}

void CommandScheduler::SetDefaultCommand(Subsystem* subsystem, Command* defaultCommand) {
  m_subsystems[subsystem] = defaultCommand;
}

Command* CommandScheduler::GetDefaultCommand(Subsystem* subsystem) const {
      auto find = m_subsystems.find(subsystem);
    if (find != m_subsystems.end()) {
      return find->second;
    }
    return nullptr;
}

void CommandScheduler::CancelAll() {
  for (auto&& command : m_scheduledCommands) {
      CancelCommands({command.first});
    }
}
