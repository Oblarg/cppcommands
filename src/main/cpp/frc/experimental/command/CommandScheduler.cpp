#include <frc/experimental/command/CommandScheduler.h>
#include <frc/WPIErrors.h>
#include <frc/experimental/command/Subsystem.h>
#include <frc/experimental/command/CommandGroupBase.h>
#include <frc/commands/Scheduler.h>
#include <frc/smartdashboard/SendableBuilder.h>
#include <hal/HAL.h>

using namespace frc::experimental;

template<typename TMap, typename TKey>
static bool ContainsKey(const TMap& map, TKey keyToCheck) {
  return map.find(keyToCheck) != map.end();
}

template<typename T1, typename T2, typename TKey>
static bool Disjoint(const T1& first, const T2& second, wpi::SmallVectorImpl<TKey>& intersection, bool& allInterruptible) {

  return false;
}

CommandScheduler::CommandScheduler() {
  SetName("Scheduler");
}

CommandScheduler& CommandScheduler::GetInstance() {
  static CommandScheduler scheduler;
  return scheduler;
}

void CommandScheduler::Schedule(bool interruptible, Command* command) {
  auto& groupedCommands = CommandGroupBase::GetGroupedCommands();
  if (ContainsKey(groupedCommands, command)) {
    wpi_setWPIErrorWithContext(CommandIllegalUse,
        "A command that is part of a command group cannot be independently scheduled");
    return;
  }
  if (m_disabled || (RobotState::IsDisabled() && !command->RunsWhenDisabled()) || ContainsKey(m_scheduledCommands, command)) {
    return;
  }
  
  const auto& requirements = command->GetRequirements();

  wpi::SmallVector<Command*, 8> intersection;

  bool isDisjoint = true;
  bool allInterruptible = true;
  for (auto&& i1 : m_requirements) {
    for (auto&& i2 : requirements) {
      if (i1.first == i2) {
        isDisjoint = false;
        allInterruptible &= m_scheduledCommands[i1.second].IsInterruptible();
        intersection.emplace_back(i1.second);
      }
    }
  }

  if (isDisjoint || allInterruptible) {
    if (allInterruptible) {
      for (auto&& cmdToCancel : intersection) {
        Cancel(cmdToCancel);
      }
    }
    command->Initialize();
    auto find = m_scheduledCommands.find(command);
    m_scheduledCommands[command] = CommandState{interruptible};
    for (auto&& action : m_initActions) {
      action(*command);
    }
    for (auto&& requirement : requirements) {
      m_requirements[requirement] = command;
    }
  }
}

void CommandScheduler::Run() {
  if(m_disabled) {
    return;
  }

  //Run the periodic method of all registered subsystems.
  for(auto&& subsystem : m_subsystems) {
    subsystem.getFirst()->Periodic();
  }

  //Poll buttons for new commands to add.
  for (auto&& button : m_buttons) {
    button();
  }

  //Run scheduled commands, remove finished commands.
  for (auto iterator = m_scheduledCommands.begin(); iterator != m_scheduledCommands.end(); iterator++) {
    Command* command = iterator->getFirst();

    if (!command->RunsWhenDisabled() && RobotState::IsDisabled()) {
      Cancel(command);
      continue;
    }

    command->Execute();
    for (auto&& action : m_executeActions) {
      action(*command);
    }

    if (command->IsFinished()) {
      command->End(false);
      for (auto&& action : m_finishActions) {
        action(*command);
      }

      for (auto&& requirement : command->GetRequirements()) {
        m_requirements.erase(requirement);
      }

      m_scheduledCommands.erase(iterator);
    }
  }

  //Add default commands for un-required registered subsystems.
  for (auto&& subsystem : m_subsystems) {
    auto s = m_requirements.find(subsystem.getFirst());
    if (s == m_requirements.end()) {
      Schedule(subsystem.getSecond());
    }
  }
}

void CommandScheduler::RegisterSubsystem(Subsystem* subsystem) {
  m_subsystems[subsystem] = nullptr;
}

void CommandScheduler::UnregisterSubsystem(Subsystem* subsystem) {
  auto s = m_subsystems.find(subsystem);
  if (s != m_subsystems.end()) {
    m_subsystems.erase(s);
  }
}

void CommandScheduler::RegisterSubsystem(wpi::ArrayRef<Subsystem*> subsystems) {
  for (auto* subsystem : subsystems) {
      RegisterSubsystem(subsystem);
    }
}

void CommandScheduler::UnregisterSubsystem(wpi::ArrayRef<Subsystem*> subsystems) {
    for (auto* subsystem : subsystems) {
      UnregisterSubsystem(subsystem);
    }
}

void CommandScheduler::SetDefaultCommand(Subsystem* subsystem, Command* defaultCommand) {
  if (!defaultCommand->HasRequirement(subsystem)) {
    wpi_setWPIErrorWithContext(CommandIllegalUse,
        "Default commands must require their subsystem!");
    return;
  }
  if (defaultCommand->IsFinished()) {
    wpi_setWPIErrorWithContext(CommandIllegalUse,
        "Default commands should not end!");
    return;
  }
  m_subsystems[subsystem] = defaultCommand;
}

Command* CommandScheduler::GetDefaultCommand(const Subsystem* subsystem) const {
  auto find = m_subsystems.find(subsystem);
    if (find != m_subsystems.end()) {
      return find->second;
    }
    else {
      return nullptr;
    }
}

void CommandScheduler::Cancel(Command* command) {
  auto find = m_scheduledCommands.find(command);
  if (find == m_scheduledCommands.end()) return;
  command->End(true);
  for (auto&& action : m_interruptActions) {
    action(*command);
  }
  m_scheduledCommands.erase(find);
  const auto& requirements = command->GetRequirements();
  for (auto requirement : requirements) {
    m_requirements.erase(requirement);
  }
}

void CommandScheduler::CancelAll() {
  for (auto&& command : m_scheduledCommands) {
      Cancel(command.first);
  }
}

  double CommandScheduler::TimeSinceScheduled(const Command* command) const {
    auto find = m_scheduledCommands.find(command);
    if (find != m_scheduledCommands.end()) {
      return find->second.TimeSinceInitialized();
    } else {
      return -1;
    }
  }
  bool CommandScheduler::IsScheduled(wpi::ArrayRef<const Command*> commands) const {
    for (auto command : commands) {
      if (!IsScheduled(command)) {
        return false;
      }
    }
    return true;
  }

  bool CommandScheduler::IsScheduled(const Command* command) const {
    return m_scheduledCommands.find(command) != m_scheduledCommands.end();
  }

Command* CommandScheduler::Requiring(const Subsystem* subsystem) const {
  auto find = m_requirements.find(subsystem);
  if (find != m_requirements.end()) {
    return find->second;
  }
  else {
    return nullptr;
  }
}

void CommandScheduler::InitSendable(frc::SendableBuilder& builder) {
  builder.SetSmartDashboardType("Scheduler");
  m_namesEntry = builder.GetEntry("Names");
  m_idsEntry = builder.GetEntry("Ids");
  m_cancelEntry = builder.GetEntry("Cancel");

  builder.SetUpdateTable([this]{
    double tmp[1];
    tmp[0] = 0;
    auto toCancel = m_cancelEntry.GetDoubleArray(tmp);
    for (auto cancel : toCancel) {
      uintptr_t ptrTmp = static_cast<uintptr_t>(cancel);
      Command* command = reinterpret_cast<Command*>(ptrTmp);
      if (m_scheduledCommands.find(command) != m_scheduledCommands.end()) {
        Cancel(command);
      }
      m_cancelEntry.SetDoubleArray(wpi::ArrayRef<double>{});
    }

    wpi::SmallVector<std::string, 8> names;
    wpi::SmallVector<double, 8>  ids;
    for (auto&& command : m_scheduledCommands) {
      names.emplace_back(command.first->GetName());
      uintptr_t ptrTmp = reinterpret_cast<uintptr_t>(command.first);
      ids.emplace_back(static_cast<double>(ptrTmp));
    }
    m_namesEntry.SetStringArray(names);
    m_idsEntry.SetDoubleArray(ids);
  });
}
