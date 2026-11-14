
// group-63

#include "grp_63_SuCourse.h"
#include "grp_63_Exceptions.h"

namespace acaderp {

SuCourse::SuCourse(
    const std::string& code,
    int credits,
    const std::string& category,
    const std::vector<std::string>& prerequisites,
    const std::vector<std::string>& antiRequisites
)
    : Course(code, credits, category, prerequisites, antiRequisites) {}

void SuCourse::validateGrade(const Grade& grade) const {
    const std::string& symbol = grade.symbol();

    if (symbol == "S" || symbol == "X" || symbol == "I") {
        return;
    }

    throw GradeTypeMismatchException(
        "Grade " + symbol + " is not valid for an SU course"
    );
}

bool SuCourse::countsInGpa(const Grade& grade) const {
    (void)grade;
    return false;
}

bool SuCourse::earnsCredits(const Grade& grade) const {
    return grade.symbol() == "S";
}

} // namespace acaderp
