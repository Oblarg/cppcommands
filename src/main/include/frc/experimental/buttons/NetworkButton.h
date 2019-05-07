#pragma once
#include "Button.h"
#include <networktables/NetworkTableEntry.h>
#include <networktables/NetworkTableInstance.h>
#include <networktables/NetworkTable.h>

namespace frc {
namespace experimental {
class NetworkButton : public Button {
 public:
  NetworkButton(const wpi::Twine& table, const wpi::Twine& field) : NetworkButton{nt::NetworkTableInstance::GetDefault().GetTable(table), field} {}
  NetworkButton(const std::shared_ptr<nt::NetworkTable>& table, const wpi::Twine& field) : m_entry{table->GetEntry(field)} {}
 private:
  nt::NetworkTableEntry m_entry;
};
}
}
