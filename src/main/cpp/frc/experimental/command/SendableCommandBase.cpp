#include <frc/experimental/command/SendableCommandBase.h>

using namespace frc::experimental;

SendableCommandBase::SendableCommandBase() {
  m_name = Command::GetName();
  m_subsystem = "Unknown";
}
