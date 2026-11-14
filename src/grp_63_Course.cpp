
// group-63

#include "grp_63_Course.h"

#include <utility>

namespace acaderp {

Course::Course(
    const std::string& code,
    int credits,
    const std::string& category,
    const std::vector<std::string>& prerequisites,
    const std::vector<std::string>& antiRequisites
)
    : code_(code),
      credits_(credits),
      category_(category),
      prerequisites_(prerequisites),
      antiRequisites_(antiRequisites) {}

const std::string& Course::code() const noexcept {
    return code_;
}

int Course::credits() const noexcept {
    return credits_;
}

const std::string& Course::category() const noexcept {
    return category_;
}

const std::vector<std::string>& Course::prerequisites() const noexcept {
    return prerequisites_;
}

const std::vector<std::string>& Course::antiRequisites() const noexcept {
    return antiRequisites_;
}

} // namespace acaderp
