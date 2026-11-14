
// group-63
#ifndef GRP_63_THESIS_COURSE_H
#define GRP_63_THESIS_COURSE_H

#include "grp_63_Course.h"

namespace acaderp {

class ThesisCourse : public Course {
public:
    ThesisCourse(
        const std::string& code,
        int credits,
        const std::string& category,
        const std::vector<std::string>& prerequisites = {},
        const std::vector<std::string>& antiRequisites = {}
    );

    void validateGrade(const Grade& grade) const override;
    bool countsInGpa(const Grade& grade) const override;
    bool earnsCredits(const Grade& grade) const override;
};

} // namespace acaderp

#endif
