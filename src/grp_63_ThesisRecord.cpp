
// group-63

#include "grp_63_ThesisRecord.h"
#include "grp_63_Exceptions.h"

namespace acaderp {

ThesisRecord::ThesisRecord(
    const std::string& courseCode,
    const std::string& term
)
    : courseCode_(courseCode),
      term_(term),
      state_(ThesisState::REGISTERED),
      progressGrades_(),
      finalGrade_(std::nullopt) {}

const std::string& ThesisRecord::courseCode() const noexcept {
    return courseCode_;
}

const std::string& ThesisRecord::term() const noexcept {
    return term_;
}

ThesisState ThesisRecord::state() const noexcept {
    return state_;
}

const std::vector<Grade>& ThesisRecord::progressGrades() const noexcept {
    return progressGrades_;
}

const std::optional<Grade>& ThesisRecord::finalGrade() const noexcept {
    return finalGrade_;
}

void ThesisRecord::addProgressGrade(
    const Grade& grade,
    const std::string& term
) {
    if (state_ == ThesisState::TERMINATED) {
        throw ThesisTerminatedException(
            "Cannot update a terminated thesis"
        );
    }

    if (state_ == ThesisState::FINAL) {
        throw InvalidThesisTransitionException(
            "Cannot add progress grades after thesis finalization"
        );
    }

    if (grade.symbol() != "S" && grade.symbol() != "X") {
        throw GradeTypeMismatchException(
            "Thesis progress grade must be S or X"
        );
    }

    // Progress term must be after registration and previous progress.
    if (term <= term_ ||
        (!progressTerms_.empty() && term <= progressTerms_.back())) {
        throw InvalidThesisTransitionException(
            "Thesis progress terms must be strictly increasing"
        );
    }

    ThesisState nextState = ThesisState::IN_PROGRESS;

    if (grade.symbol() == "X") {
        if (!progressGrades_.empty() &&
            progressGrades_.back().symbol() == "X") {
            nextState = ThesisState::TERMINATED;
        } else {
            nextState = ThesisState::AT_RISK;
        }
    }

    // Prepare both updates before modifying the record.
    std::vector<Grade> nextGrades = progressGrades_;
    std::vector<std::string> nextTerms = progressTerms_;

    nextGrades.push_back(grade);
    nextTerms.push_back(term);

    progressGrades_.swap(nextGrades);
    progressTerms_.swap(nextTerms);
    state_ = nextState;
}

void ThesisRecord::convertToFinalGrade(const Grade& grade) {
    if (state_ == ThesisState::TERMINATED) {
        throw ThesisTerminatedException(
            "Cannot convert a terminated thesis"
        );
    }

    if (state_ != ThesisState::IN_PROGRESS) {
        throw InvalidThesisTransitionException(
            "Thesis can only be converted from IN_PROGRESS"
        );
    }

    const std::string& symbol = grade.symbol();

    if (symbol != "A" && symbol != "A-" &&
        symbol != "B" && symbol != "B-" &&
        symbol != "C" && symbol != "C-" &&
        symbol != "D" && symbol != "E" &&
        symbol != "F") {
        throw GradeTypeMismatchException(
            "Thesis conversion requires a letter grade"
        );
    }

    finalGrade_ = grade;
    state_ = ThesisState::FINAL;
}

} // namespace acaderp
