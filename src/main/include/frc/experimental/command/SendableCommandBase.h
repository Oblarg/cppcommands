#pragma once

#include <frc/smartdashboard/Sendable.h>
#include "Command.h"
#include <wpi/SmallVector.h>
#include <wpi/SmallPtrSet.h>

namespace frc {
namespace experimental {
class SendableCommandBase : public Sendable, public Command {
 public:
  SendableCommandBase(SendableCommandBase&& other) = default;

  void AddRequirements(wpi::ArrayRef<Subsystem*> requirements);

  wpi::ArrayRef<Subsystem*> GetRequirements() const override;

  void SetName(const wpi::Twine& name) override {
    m_name = name.str();
  }

  std::string GetName() const override {
    return m_name;
  }

  std::string GetSubsystem() const override {
    return m_subsystem;
  }

  void SetSubsystem(const wpi::Twine& subsystem) override {
    m_subsystem = subsystem.str();
  }

  void InitSendable(frc::SendableBuilder& builder) override;


 protected:
  SendableCommandBase();
  std::string m_name;
  std::string m_subsystem;
  wpi::SmallVector<Subsystem*, 4> m_requirements;
};
}
}
