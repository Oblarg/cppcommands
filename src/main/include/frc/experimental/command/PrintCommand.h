#pragma once

#include "InstantCommand.h"
#include "CommandHelper.h"
#include "wpi/Twine.h"
#include "wpi/raw_ostream.h"

namespace frc {
namespace experimental {
class PrintCommand : public CommandHelper<InstantCommand, PrintCommand>  {
 public:
  explicit PrintCommand(const wpi::Twine& message) 
    : CommandHelper{[str = message.str()]{
      wpi::outs() << str << "\n";
    }, {}} {
    }

  PrintCommand(PrintCommand&& other) = default;
    
  bool RunsWhenDisabled() const override {
    return true;
  }
};
}
}
