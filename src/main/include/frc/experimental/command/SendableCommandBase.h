#pragma once

#include <frc/smartdashboard/Sendable.h>
#include "Command.h"
#include <set>

namespace frc {
namespace experimental {
class SendableCommandBase : public Sendable, public Command {
 public:
  void AddRequirements(std::initializer_list<Subsystem*> requirements);
  void AddRequirements(const std::set<Subsystem*>& requirements);
  template<typename T>
  void AddRequirements(T begin, T end) {
    m_requirements.insert(begin, end);
  }

  const std::set<Subsystem*>& GetRequirements() const override;

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
  std::set<Subsystem*> m_requirements;
};
}
}
