
// group-63

#include "grp_63_RegularCourse.h"
#include "grp_63_Exceptions.h"

namespace acaderp {

RegularCourse::RegularCourse(
    const std::string& code,
    int credits,
    const std::string& category,
    const std::vector<std::string>& prerequisites,
    const std::vector<std::string>& antiRequisites
)
    : Course(code, credits, category, prerequisites, antiRequisites) {}

void RegularCourse::validateGrade(const Grade& grade) const {
    const std::string& symbol = grade.symbol();

    if (symbol == "A" || symbol == "A-" ||
        symbol == "B" || symbol == "B-" ||
        symbol == "C" || symbol == "C-" ||
        symbol == "D" || symbol == "E" ||
        symbol == "F" || symbol == "I") {
        return;
    }

    throw GradeTypeMismatchException(
        "Grade " + symbol + " is not valid for a regular course"
    );
}

bool RegularCourse::countsInGpa(const Grade& grade) const {
    return grade.isFinal() &&
           grade.symbol() != "I";
}

bool RegularCourse::earnsCredits(const Grade& grade) const {
    return grade.isFinal() && grade.isPass();
}

} // namespace acaderp
