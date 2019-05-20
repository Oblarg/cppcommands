#pragma once

#include "InstantCommand.h"
#include "wpi/Twine.h"
#include "wpi/raw_ostream.h"

namespace frc {
namespace experimental {
class PrintCommand : public InstantCommand {
 public:
  explicit PrintCommand(const wpi::Twine& message) 
    : InstantCommand{[str = message.str()]{
      wpi::outs() << str << "\n";
    }, {}} {
    }
    
  bool RunsWhenDisabled() override {
    return true;
  }
};
}
}
