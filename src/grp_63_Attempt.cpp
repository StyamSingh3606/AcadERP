// group-63

#include "grp_63_Attempt.h"
#include "grp_63_Exceptions.h"

namespace acaderp {

Attempt::Attempt(
    const std::string& courseCode,
    const std::string& term,
    int attemptNumber,
    const Grade& grade
)
    : courseCode_(courseCode),
      term_(term),
      attemptNumber_(attemptNumber),
      grade_(grade) {}

const std::string& Attempt::courseCode() const noexcept {
    return courseCode_;
}

const std::string& Attempt::term() const noexcept {
    return term_;
}

int Attempt::attemptNumber() const noexcept {
    return attemptNumber_;
}

const Grade& Attempt::grade() const noexcept {
    return grade_;
}

void Attempt::updateGrade(const Grade& grade) {
    if (grade_.symbol() != "I") {
        throw GradeAlreadyFinalException(
            "Cannot change a final grade"
        );
    }

    if (grade.symbol() == "I") {
        throw GradeAlreadyFinalException(
            "Temporary grade I can only be replaced by a final grade"
        );
    }

    grade_ = grade;
}

} // namespace acaderp
