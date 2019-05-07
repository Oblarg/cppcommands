#include "frc/experimental/command/Command.h"

#ifdef __GNUG__
#include <cstdlib>
#include <memory>
#include <cxxabi.h>

namespace frc {
namespace experimental {
std::string demange(const char* name) {
    int status = -1;

    std::unique_ptr<char, void(*)(void*)> res {
        abi::__cxa_demangle(name, NULL, NULL, &status),
        std::free
    };

    return (status==0) ? res.get() : name ;
}
}
}

#else

namespace frc {
namespace experimental {
std::string demangle(const char* name) {
  return name;
}
}
}

#endif

using namespace frc::experimental;

void Command::Initialize() {}
void Command::Execute() {}
void Command::End(bool interrupted) {}

void Command::Schedule(bool interruptible) {

}

void Command::Cancel() {

}

bool Command::IsScheduled() const {
  return false;
}

bool Command::HasRequirement(Subsystem* requirement) const {
  return true;
}

std::string Command::GetName() const {
  return GetTypeName(*this);
}


