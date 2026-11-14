// group-63

#include "grp_63_Student.h"

namespace acaderp {

Student::Student(
    const std::string& id,
    const std::string& name,
    const std::string& programCode
)
    : id_(id),
      name_(name),
      programCode_(programCode) {}

const std::string& Student::id() const noexcept {
    return id_;
}

const std::string& Student::name() const noexcept {
    return name_;
}

const std::string& Student::programCode() const noexcept {
    return programCode_;
}

const std::vector<Attempt>& Student::attempts() const noexcept {
    return attempts_;
}

void Student::addAttempt(const Attempt& attempt) {
    attempts_.push_back(attempt);
}

Attempt* Student::findLatestAttempt(const std::string& courseCode) {
    for (auto it = attempts_.rbegin(); it != attempts_.rend(); ++it) {
        if (it->courseCode() == courseCode) {
            return &(*it);
        }
    }

    return nullptr;
}

const Attempt* Student::findLatestAttempt(
    const std::string& courseCode
) const {
    for (auto it = attempts_.rbegin(); it != attempts_.rend(); ++it) {
        if (it->courseCode() == courseCode) {
            return &(*it);
        }
    }

    return nullptr;
}

} // namespace acaderp
