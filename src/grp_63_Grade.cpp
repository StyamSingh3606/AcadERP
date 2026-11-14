// group-63

#include "grp_63_Grade.h"
#include "grp_63_Exceptions.h"

namespace acaderp {

Grade::Grade(const std::string& symbol)
    : symbol_(symbol) {}

Grade Grade::create(const std::string& symbol) {
    static const std::string validGrades[] = {
        "A", "A-", "B", "B-", "C", "C-", "D", "E", "F",
        "I", "S", "X"
    };

    for (const std::string& valid : validGrades) {
        if (symbol == valid) {
            return Grade(symbol);
        }
    }

    throw InvalidGradeSymbolException(symbol);
}

const std::string& Grade::symbol() const noexcept {
    return symbol_;
}

int Grade::pointsHundredths() const {
    if (symbol_ == "A")  return 1000;
    if (symbol_ == "A-") return 900;
    if (symbol_ == "B")  return 800;
    if (symbol_ == "B-") return 700;
    if (symbol_ == "C")  return 600;
    if (symbol_ == "C-") return 500;
    if (symbol_ == "D")  return 400;
    if (symbol_ == "E")  return 200;
    if (symbol_ == "F")  return 0;

    throw GradeTypeMismatchException(
        "Grade " + symbol_ + " does not have GPA points"
    );
}

bool Grade::isPass() const noexcept {
    return symbol_ == "A" || symbol_ == "A-" ||
           symbol_ == "B" || symbol_ == "B-" ||
           symbol_ == "C" || symbol_ == "C-" ||
           symbol_ == "D" || symbol_ == "S";
}

bool Grade::isFinal() const noexcept {
    return symbol_ != "I";
}

} // namespace acaderp