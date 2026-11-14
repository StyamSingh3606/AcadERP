// group-63
#ifndef GRP_63_STUDENT_H
#define GRP_63_STUDENT_H

#include <string>
#include <vector>

#include "grp_63_Attempt.h"

namespace acaderp {

class Student {
private:
    std::string id_;
    std::string name_;
    std::string programCode_;
    std::vector<Attempt> attempts_;

public:
    Student(
        const std::string& id,
        const std::string& name,
        const std::string& programCode
    );

    const std::string& id() const noexcept;
    const std::string& name() const noexcept;
    const std::string& programCode() const noexcept;

    const std::vector<Attempt>& attempts() const noexcept;

    void addAttempt(const Attempt& attempt);

    Attempt* findLatestAttempt(const std::string& courseCode);
    const Attempt* findLatestAttempt(
        const std::string& courseCode
    ) const;
};

} // namespace acaderp

#endif
