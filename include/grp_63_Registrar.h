// group-63

#ifndef GRP_63_REGISTRAR_H
#define GRP_63_REGISTRAR_H

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "grp_63_Course.h"
#include "grp_63_Student.h"
#include "grp_63_ThesisRecord.h"
#include "grp_63_ProgramRules.h"

namespace acaderp {

class Registrar {
private:
    std::map<std::string, std::unique_ptr<Course>> courses_;
    std::map<std::string, Student> students_;
    std::map<std::string, ThesisRecord> thesisRecords_;
    ProgramRules rules_;

    std::string thesisKey(
        const std::string& studentId,
        const std::string& courseCode,
        const std::string& term
    ) const;

public:
    Registrar();

    void setProgramRules(const ProgramRules& rules);

    void addCourse(std::unique_ptr<Course> course);
    void addStudent(
        const std::string& id,
        const std::string& name
    );

    void registerCourse(
        const std::string& studentId,
        const std::string& courseCode,
        const std::string& term
    );

    void submitGrade(
        const std::string& studentId,
        const std::string& courseCode,
        const std::string& term,
        const std::string& symbol
    );

    void convertThesis(
        const std::string& studentId,
        const std::string& courseCode,
        const std::string& symbol
    );

    int calculateGpaHundredths(const std::string& studentId) const;
    std::string calculateAverage(const std::string& studentId) const;
    std::string generateTranscript(const std::string& studentId) const;
    std::vector<std::string> graduationFailures(
        const std::string& studentId
    ) const;

    std::string eligibilityFailure(
        const std::string& studentId,
        const std::string& courseCode,
        const std::string& term
    ) const;

    const Student& getStudent(const std::string& studentId) const;
    const Course& getCourse(const std::string& courseCode) const;

    const ProgramRules& programRules() const noexcept;
};

} // namespace acaderp

#endif
