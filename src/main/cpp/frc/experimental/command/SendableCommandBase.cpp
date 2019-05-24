#include <frc/experimental/command/SendableCommandBase.h>
#include <frc/smartdashboard/SendableBuilder.h>
#include <frc/experimental/command/SetUtilities.h>

using namespace frc::experimental;

SendableCommandBase::SendableCommandBase() {
  m_name = Command::GetName();
  m_subsystem = "Unknown";
}


  void SendableCommandBase::AddRequirements(wpi::ArrayRef<Subsystem*> requirements) {
    SetInsert(m_requirements, requirements);
  }

  wpi::ArrayRef<Subsystem*> SendableCommandBase::GetRequirements() const {
    return wpi::ArrayRef<Subsystem*>{m_requirements.begin(), m_requirements.end()};
  }

  void SendableCommandBase::InitSendable(frc::SendableBuilder& builder) {
    builder.SetSmartDashboardType("Command");
    builder.AddStringProperty(".name", [this]{return GetName();}, nullptr);
    builder.AddBooleanProperty("running", [this] {return IsScheduled();},
    [this](bool value){
      bool isScheduled = IsScheduled();
      if (value && !isScheduled) {
        Schedule(true);
      } else if (!value && isScheduled) {
        Cancel();
      }
    });
  }
