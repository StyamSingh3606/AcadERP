
// group-63
#ifndef GRP_63_COURSE_H
#define GRP_63_COURSE_H

#include <string>
#include <vector>

#include "grp_63_Grade.h"

namespace acaderp {

class Course {
private:
    std::string code_;
    int credits_;
    std::string category_;
    std::vector<std::string> prerequisites_;
    std::vector<std::string> antiRequisites_;

protected:
    Course(
        const std::string& code,
        int credits,
        const std::string& category,
        const std::vector<std::string>& prerequisites,
        const std::vector<std::string>& antiRequisites
    );

public:
    virtual ~Course() = default;

    const std::string& code() const noexcept;
    int credits() const noexcept;
    const std::string& category() const noexcept;

    const std::vector<std::string>& prerequisites() const noexcept;
    const std::vector<std::string>& antiRequisites() const noexcept;

    virtual void validateGrade(const Grade& grade) const = 0;
    virtual bool countsInGpa(const Grade& grade) const = 0;
    virtual bool earnsCredits(const Grade& grade) const = 0;
};

} // namespace acaderp

#endif
