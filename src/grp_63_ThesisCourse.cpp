
// group-63

#include "grp_63_ThesisCourse.h"
#include "grp_63_Exceptions.h"

namespace acaderp {

ThesisCourse::ThesisCourse(
    const std::string& code,
    int credits,
    const std::string& category,
    const std::vector<std::string>& prerequisites,
    const std::vector<std::string>& antiRequisites
)
    : Course(code, credits, category, prerequisites, antiRequisites) {}

void ThesisCourse::validateGrade(const Grade& grade) const {
    const std::string& symbol = grade.symbol();

    if (symbol == "S" || symbol == "X") {
        return;
    }

    throw GradeTypeMismatchException(
        "Grade " + symbol + " is not valid as a thesis progress grade"
    );
}

bool ThesisCourse::countsInGpa(const Grade& grade) const {
    const std::string& symbol = grade.symbol();

    return symbol == "A" || symbol == "A-" ||
           symbol == "B" || symbol == "B-" ||
           symbol == "C" || symbol == "C-" ||
           symbol == "D" || symbol == "E" ||
           symbol == "F";
}

bool ThesisCourse::earnsCredits(const Grade& grade) const {
    const std::string& symbol = grade.symbol();

    return symbol == "A" || symbol == "A-" ||
           symbol == "B" || symbol == "B-" ||
           symbol == "C" || symbol == "C-" ||
           symbol == "D";
}

} // namespace acaderp
