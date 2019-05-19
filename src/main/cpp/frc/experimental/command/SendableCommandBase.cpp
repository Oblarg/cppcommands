#include <frc/experimental/command/SendableCommandBase.h>
#include <frc/smartdashboard/SendableBuilder.h>

using namespace frc::experimental;

SendableCommandBase::SendableCommandBase() {
  m_name = Command::GetName();
  m_subsystem = "Unknown";
}


  void SendableCommandBase::AddRequirements(std::initializer_list<Subsystem*> requirements) {
    m_requirements.insert(requirements);
  }

  const std::set<Subsystem*>& SendableCommandBase::GetRequirements() const {
    return m_requirements;
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
