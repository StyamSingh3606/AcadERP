// group-63

// group-63

#include "grp_63_ProgramRules.h"

namespace acaderp {

ProgramRules::ProgramRules()
    : totalCredits_(160),
      electiveCredits_(24),
      minimumGpaHundredths_(500),
      thesisMinimumGrade_("C"),
      thesisMinimumCredits_(100),
      thesisMinimumGpaHundredths_(600),
      maximumLoad_(24) {}

int ProgramRules::totalCredits() const noexcept {
    return totalCredits_;
}

int ProgramRules::electiveCredits() const noexcept {
    return electiveCredits_;
}

int ProgramRules::minimumGpaHundredths() const noexcept {
    return minimumGpaHundredths_;
}

const std::string& ProgramRules::thesisMinimumGrade() const noexcept {
    return thesisMinimumGrade_;
}

int ProgramRules::thesisMinimumCredits() const noexcept {
    return thesisMinimumCredits_;
}

int ProgramRules::thesisMinimumGpaHundredths() const noexcept {
    return thesisMinimumGpaHundredths_;
}

int ProgramRules::maximumLoad() const noexcept {
    return maximumLoad_;
}

void ProgramRules::setTotalCredits(int value) {
    totalCredits_ = value;
}

void ProgramRules::setElectiveCredits(int value) {
    electiveCredits_ = value;
}

void ProgramRules::setMinimumGpaHundredths(int value) {
    minimumGpaHundredths_ = value;
}

void ProgramRules::setThesisMinimumGrade(const std::string& value) {
    thesisMinimumGrade_ = value;
}

void ProgramRules::setThesisMinimumCredits(int value) {
    thesisMinimumCredits_ = value;
}

void ProgramRules::setThesisMinimumGpaHundredths(int value) {
    thesisMinimumGpaHundredths_ = value;
}

void ProgramRules::setMaximumLoad(int value) {
    maximumLoad_ = value;
}

} // namespace acaderp
