#include "CommandTestBase.h"

namespace frc {
namespace experimental {

CommandTestBase::CommandTestBase() {
  auto& scheduler = CommandScheduler::GetInstance();
  scheduler.CancelAll();
  scheduler.Enable();
  scheduler.ClearButtons();
}

CommandScheduler CommandTestBase::GetScheduler() {
  return CommandScheduler();
};

void CommandTestBase::SetUp() {
  HALSIM_SetDriverStationEnabled(true);
  while (!HALSIM_GetDriverStationEnabled()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

void CommandTestBase::TearDown() {
}

void CommandTestBase::SetDSEnabled(bool enabled){
  HALSIM_SetDriverStationEnabled(enabled);
  while (HALSIM_GetDriverStationEnabled() != enabled) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

}
}