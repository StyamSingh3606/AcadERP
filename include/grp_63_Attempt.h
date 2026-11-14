
// group-63
#ifndef GRP_63_ATTEMPT_H
#define GRP_63_ATTEMPT_H

#include <string>

#include "grp_63_Grade.h"

namespace acaderp {

class Attempt {
private:
    std::string courseCode_;
    std::string term_;
    int attemptNumber_;
    Grade grade_;

public:
    Attempt(
        const std::string& courseCode,
        const std::string& term,
        int attemptNumber,
        const Grade& grade
    );

    const std::string& courseCode() const noexcept;
    const std::string& term() const noexcept;
    int attemptNumber() const noexcept;
    const Grade& grade() const noexcept;

    void updateGrade(const Grade& grade);
};

} // namespace acaderp

#endif
