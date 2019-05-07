#include <frc/experimental/command/SendableSubsystemBase.h>
#include <frc/experimental/command/Command.h>
#include <frc/experimental/command/CommandScheduler.h>

using namespace frc::experimental;

SendableSubsystemBase::SendableSubsystemBase() {
  m_name = GetTypeName(*this);
  CommandScheduler::GetInstance().RegisterSubsystem({this});
}

