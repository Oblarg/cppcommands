#include <frc/experimental/buttons/Trigger.h>
#include <frc/experimental/command/CommandScheduler.h>
#include <frc/experimental/command/Command.h>
#include <frc/experimental/command/InstantCommand.h>
#include <frc/smartdashboard/SendableBuilder.h>

using namespace frc::experimental;

Trigger* Trigger::WhenActive(Command* command, bool interruptible) {
  CommandScheduler::GetInstance().AddButton([pressedLast = Grab(), this, command, interruptible]() mutable {
    bool pressed = Grab();

    if (!pressedLast && pressed) {
      command->Schedule(interruptible);
    }

    pressedLast = pressed;
  });

  return this;
}

Trigger* Trigger::WhenActive(std::function<void()> toRun) {
  return WhenActive(new InstantCommand(std::move(toRun), {}));
}

Trigger* Trigger::WhileActiveContinous(Command* command, bool interruptible) {
  CommandScheduler::GetInstance().AddButton([pressedLast = Grab(), this, command, interruptible]() mutable {
    bool pressed = Grab();

    if (pressed) {
      command->Schedule(interruptible);
    } else if (pressedLast && !pressed) {
      command->Cancel();
    }

    pressedLast = pressed;
  });
  return this;
}

Trigger* Trigger::WhileActiveContinous(std::function<void()> toRun) {
  return WhileActiveContinous(new InstantCommand(std::move(toRun), {}));
}

Trigger* Trigger::WhileActiveOnce(Command* command, bool interruptible) {
  CommandScheduler::GetInstance().AddButton([pressedLast = Grab(), this, command, interruptible]() mutable {
    bool pressed = Grab();

    if (!pressedLast && pressed) {
      command->Schedule(interruptible);
    } else if (pressedLast && !pressed) {
      command->Cancel();
    }

    pressedLast = pressed;
  });
  return this;
}

Trigger* Trigger::WhenInactive(Command* command, bool interruptible) {
  CommandScheduler::GetInstance().AddButton([pressedLast = Grab(), this, command, interruptible]() mutable {
    bool pressed = Grab();

    if (pressedLast && !pressed) {
      command->Schedule(interruptible);
    }

    pressedLast = pressed;
  });
  return this;
}

Trigger* Trigger::WhenInactive(std::function<void()> toRun) {
  return WhenInactive(new InstantCommand(std::move(toRun), {}));
}

Trigger* Trigger::ToggleWhenActive(Command* command, bool interruptible) {
  CommandScheduler::GetInstance().AddButton([pressedLast = Grab(), this, command, interruptible]() mutable {
    bool pressed = Grab();

    if (!pressedLast && pressed) {
      if (command->IsScheduled()) {
        command->Cancel();
      } else {
        command->Schedule(interruptible);
      }
    }

    pressedLast = pressed;
  });
  return this;
}

Trigger* Trigger::CancelWhenActive(Command* command) {
  CommandScheduler::GetInstance().AddButton([pressedLast = Grab(), this, command]() mutable {
    bool pressed = Grab();

    if (!pressedLast && pressed) {
      command->Cancel();
    }

    pressedLast = pressed;
  });
  return this;
}

void Trigger::InitSendable(frc::SendableBuilder& builder) {
  builder.SetSmartDashboardType("Button");
  builder.SetSafeState([this]{ m_sendablePressed = false; });
  builder.AddBooleanProperty("pressed", [this]{return Grab();}, [this](bool value){
    m_sendablePressed = value;
  });
}
