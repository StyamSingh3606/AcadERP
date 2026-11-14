
// group-63
#ifndef GRP_63_THESIS_RECORD_H
#define GRP_63_THESIS_RECORD_H

#include <string>
#include <vector>
#include <optional>

#include "grp_63_Grade.h"

namespace acaderp {

enum class ThesisState {
    REGISTERED,
    IN_PROGRESS,
    AT_RISK,
    TERMINATED,
    FINAL
};

class ThesisRecord {
private:
    std::string courseCode_;
    std::string term_;
    ThesisState state_;
    std::vector<Grade> progressGrades_;
    std::vector<std::string> progressTerms_;
    std::optional<Grade> finalGrade_;

public:
    ThesisRecord(
        const std::string& courseCode,
        const std::string& term
    );

    const std::string& courseCode() const noexcept;
    const std::string& term() const noexcept;
    ThesisState state() const noexcept;

    const std::optional<Grade>& finalGrade() const noexcept;
    const std::vector<Grade>& progressGrades() const noexcept;

    void addProgressGrade(const Grade& grade);
    void convertToFinalGrade(const Grade& grade);
};

} // namespace acaderp

#endif
