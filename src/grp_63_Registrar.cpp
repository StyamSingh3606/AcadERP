
// group-63

#include "grp_63_Registrar.h"

#include <algorithm>
#include <sstream>
#include <utility>

#include "grp_63_Exceptions.h"
#include "grp_63_Grade.h"
#include "grp_63_RegularCourse.h"
#include "grp_63_SuCourse.h"
#include "grp_63_ThesisCourse.h"

namespace acaderp {

Registrar::Registrar() = default;

std::string Registrar::thesisKey(
    const std::string& studentId,
    const std::string& courseCode,
    const std::string& term
) const {
    return studentId + "|" + courseCode + "|" + term;
}

void Registrar::setProgramRules(const ProgramRules& rules) {
    rules_ = rules;
}

void Registrar::addCourse(std::unique_ptr<Course> course) {
    if (!course) {
        throw InvalidCourseException("Cannot add a null course");
    }

    const std::string code = course->code();

    if (courses_.find(code) != courses_.end()) {
        throw DuplicateEntityException(code);
    }

    for (const std::string& prerequisite : course->prerequisites()) {
        const std::size_t colon = prerequisite.find(':');

        const std::string prerequisiteCode = prerequisite.substr(0, colon);

        if (courses_.find(prerequisiteCode) == courses_.end()) {
            throw InvalidCourseException(
                "Unknown prerequisite: " + prerequisiteCode
            );
        }
    }

    courses_.emplace(code, std::move(course));
}

void Registrar::addStudent(
    const std::string& id,
    const std::string& name
) {
    if (students_.find(id) != students_.end()) {
        throw DuplicateEntityException(id);
    }

    students_.emplace(id, Student(id, name, ""));
}

const Student& Registrar::getStudent(
    const std::string& studentId
) const {
    const auto it = students_.find(studentId);

    if (it == students_.end()) {
        throw UnknownStudentException(studentId);
    }

    return it->second;
}

const Course& Registrar::getCourse(
    const std::string& courseCode
) const {
    const auto it = courses_.find(courseCode);

    if (it == courses_.end()) {
        throw UnknownCourseException(courseCode);
    }

    return *it->second;
}

const ProgramRules& Registrar::programRules() const noexcept {
    return rules_;
}

void Registrar::registerCourse(
    const std::string& studentId,
    const std::string& courseCode,
    const std::string& term
) {
    (void)getStudent(studentId);

    Student& student = students_.find(studentId)->second;
    const Course& course = getCourse(courseCode);

    const std::string failure =
        eligibilityFailure(studentId, courseCode, term);

    if (failure == "E2") {
        throw AlreadyPassedException(courseCode);
    }
    if (failure == "E3") {
        throw DuplicateRegistrationException(courseCode);
    }
    if (failure == "E4") {
        throw PrerequisiteNotMetException(courseCode);
    }
    if (failure == "E5") {
        throw AntiRequisiteException(courseCode);
    }
    if (failure == "E6") {
        throw CreditLimitExceededException(courseCode);
    }
    if (failure == "E7") {
        throw ThesisNotAllowedException(courseCode);
    }

    int nextAttemptNumber = 1;
    const Attempt* latest = student.findLatestAttempt(courseCode);

    if (latest != nullptr) {
        nextAttemptNumber = latest->attemptNumber() + 1;
    }

    if (dynamic_cast<const ThesisCourse*>(&course) != nullptr) {
        const std::string key = thesisKey(studentId, courseCode, term);

        if (thesisRecords_.find(key) != thesisRecords_.end()) {
            throw DuplicateRegistrationException(courseCode);
        }

        thesisRecords_.emplace(
            key,
            ThesisRecord(courseCode, term)
        );
    } else {
        student.addAttempt(
            Attempt(
                courseCode,
                term,
                nextAttemptNumber,
                Grade::create("I")
            )
        );
    }
}

std::string Registrar::eligibilityFailure(
    const std::string& studentId,
    const std::string& courseCode,
    const std::string& term
) const {
    // E1: Student and course must exist.
    const Student& student = getStudent(studentId);
    const Course& course = getCourse(courseCode);

    // Find the latest completed attempt for this course.
    const Attempt* latestFinal = nullptr;
    int totalAttempts = 0;

    for (const Attempt& attempt : student.attempts()) {
        if (attempt.courseCode() == courseCode) {
            ++totalAttempts;

            if (attempt.grade().isFinal()) {
                latestFinal = &attempt;
            }
        }
    }

    // E2: Already passed, unless one improvement is allowed.
    if (latestFinal != nullptr && latestFinal->grade().isPass()) {
        bool improvementAllowed = false;

        if (dynamic_cast<const RegularCourse*>(&course) != nullptr &&
            (latestFinal->grade().symbol() == "C-" ||
             latestFinal->grade().symbol() == "D") &&
            totalAttempts == 1) {
            improvementAllowed = true;
        }

        if (!improvementAllowed) {
            return "E2";
        }
    }

    // E3: Check for an open registration in the same course.
    for (const Attempt& attempt : student.attempts()) {
        if (attempt.courseCode() == courseCode &&
            !attempt.grade().isFinal()) {
            return "E3";
        }
    }

    // E3 also checks open thesis registrations for the same course.
    const std::string thesisPrefix = studentId + "|" + courseCode + "|";

    for (const auto& entry : thesisRecords_) {
        if (entry.first.compare(0, thesisPrefix.size(), thesisPrefix) != 0) {
            continue;
        }

        const ThesisState state = entry.second.state();

        if (state == ThesisState::REGISTERED ||
            state == ThesisState::IN_PROGRESS ||
            state == ThesisState::AT_RISK) {
            return "E3";
        }
    }

    // E4: Check prerequisites and their minimum required grades.

    // E4: Check prerequisites and their minimum required grades.
    for (const std::string& prerequisite : course.prerequisites()) {
        const std::size_t colon = prerequisite.find(':');

        const std::string prerequisiteCode =
            prerequisite.substr(0, colon);

        const Course& prerequisiteCourse =
            getCourse(prerequisiteCode);

        const bool isSuPrerequisite =
            dynamic_cast<const SuCourse*>(&prerequisiteCourse) != nullptr;

        const std::string minimumGradeSymbol =
            (colon == std::string::npos)
                ? (isSuPrerequisite ? "S" : "D")
                : prerequisite.substr(colon + 1);

        const Grade minimumGrade =
            Grade::create(minimumGradeSymbol);

        const Attempt* prerequisiteAttempt = nullptr;

        for (const Attempt& attempt : student.attempts()) {
            if (attempt.courseCode() == prerequisiteCode &&
                attempt.grade().isFinal()) {
                if (prerequisiteAttempt == nullptr ||
                    attempt.attemptNumber() >
                    prerequisiteAttempt->attemptNumber()) {
                    prerequisiteAttempt = &attempt;
                }
            }
        }

        if (prerequisiteAttempt == nullptr ||
            !prerequisiteAttempt->grade().isPass()) {
            return "E4";
        }

        if (isSuPrerequisite) {
            if (prerequisiteAttempt->grade().symbol() != "S") {
                return "E4";
            }
        } else if (
            prerequisiteAttempt->grade().pointsHundredths() <
            minimumGrade.pointsHundredths()
        ) {
            return "E4";
        }
    }

    // E5: Check anti-requisites.
    for (const std::string& anti : course.antiRequisites()) {
        for (const Attempt& attempt : student.attempts()) {
            if (attempt.courseCode() == anti &&
                attempt.grade().isFinal() &&
                attempt.grade().isPass()) {
                return "E5";
            }
        }
    }

    // E6: Check the term credit limit.
    int termCredits = 0;

    for (const Attempt& attempt : student.attempts()) {
        if (attempt.term() == term) {
            termCredits += getCourse(attempt.courseCode()).credits();
        }
    }

    // Thesis credits count only in the thesis registration term.
    const std::string studentPrefix = studentId + "|";

    for (const auto& entry : thesisRecords_) {
        if (entry.first.compare(0, studentPrefix.size(), studentPrefix) != 0) {
            continue;
        }

        if (entry.second.term() == term) {
            termCredits += getCourse(entry.second.courseCode()).credits();
        }
    }

    if (termCredits + course.credits() > rules_.maximumLoad()) {
        return "E6";
    }

    // E7: Thesis-specific eligibility.
    if (dynamic_cast<const ThesisCourse*>(&course) != nullptr) {
        int earnedCredits = 0;

        for (const auto& entry : courses_) {
            const std::string& registeredCode = entry.first;
            const Course& registeredCourse = *entry.second;

            const Attempt* counting = nullptr;

            for (const Attempt& attempt : student.attempts()) {
                if (attempt.courseCode() == registeredCode &&
                    attempt.grade().isFinal() &&
                    (counting == nullptr ||
                     attempt.attemptNumber() > counting->attemptNumber())) {
                    counting = &attempt;
                }
            }

            if (counting != nullptr &&
                registeredCourse.earnsCredits(counting->grade())) {
                earnedCredits += registeredCourse.credits();
            }
        }

        int gpaHundredths = 0;

        try {
            gpaHundredths = calculateGpaHundredths(studentId);
        } catch (const NoGpaDataException&) {
            return "E7";
        }

        // An open thesis or an already-passed thesis blocks a new thesis.
        for (const auto& entry : thesisRecords_) {
            const std::string prefix = studentId + "|";

            if (entry.first.compare(0, prefix.size(), prefix) != 0) {
                continue;
            }

            const ThesisRecord& record = entry.second;

            if (record.state() == ThesisState::REGISTERED ||
                record.state() == ThesisState::IN_PROGRESS ||
                record.state() == ThesisState::AT_RISK) {
                return "E7";
            }

            if (record.state() == ThesisState::FINAL &&
                record.finalGrade().has_value() &&
                record.finalGrade()->isPass()) {
                return "E7";
            }

            // TERMINATED and FINAL with a failing grade do not block registration.
        }

        if (earnedCredits < rules_.thesisMinimumCredits() ||
            gpaHundredths < rules_.thesisMinimumGpaHundredths()) {
            return "E7";
        }
    }

    return "";
}

void Registrar::submitGrade(
    const std::string& studentId,
    const std::string& courseCode,
    const std::string& term,
    const std::string& symbol
) {
    (void)getStudent(studentId);

    Student& student = students_.find(studentId)->second;
    const Course& course = getCourse(courseCode);

    Grade grade = Grade::create(symbol);
    course.validateGrade(grade);

    // Thesis progress grades are managed by ThesisRecord.
   // Thesis progress grades are managed by ThesisRecord.
if (dynamic_cast<const ThesisCourse*>(&course) != nullptr) {
    const std::string prefix = studentId + "|" + courseCode + "|";

    auto latestRecord = thesisRecords_.end();

    for (auto current = thesisRecords_.begin();
         current != thesisRecords_.end(); ++current) {
        if (current->first.compare(0, prefix.size(), prefix) != 0) {
            continue;
        }

        const ThesisRecord& record = current->second;

        // Progress must be submitted after the registration term.
        if (term <= record.term()) {
            continue;
        }

        // Select the latest eligible thesis registration.
        if (latestRecord == thesisRecords_.end() ||
            record.term() > latestRecord->second.term()) {
            latestRecord = current;
        }
    }

    if (latestRecord == thesisRecords_.end()) {
        throw NotRegisteredException(courseCode);
    }

    // Let ThesisRecord raise the appropriate state-transition exception.
    latestRecord->second.addProgressGrade(grade, term);
    return;
}

    // Regular and SU course grades.
    Attempt* attempt = student.findLatestAttempt(courseCode);

    if (attempt == nullptr || attempt->term() != term) {
        throw NotRegisteredException(courseCode);
    }

    attempt->updateGrade(grade);
}

void Registrar::convertThesis(
    const std::string& studentId,
    const std::string& courseCode,
    const std::string& symbol
) {
    (void)getStudent(studentId);
    const Course& course = getCourse(courseCode);

    if (dynamic_cast<const ThesisCourse*>(&course) == nullptr) {
        throw GradeTypeMismatchException(
            "CONVERT is only allowed for thesis courses"
        );
    }

    const std::string prefix = studentId + "|" + courseCode + "|";

    auto latestRecord = thesisRecords_.end();

    for (auto current = thesisRecords_.begin();
         current != thesisRecords_.end(); ++current) {
        if (current->first.compare(0, prefix.size(), prefix) != 0) {
            continue;
        }

        if (latestRecord == thesisRecords_.end() ||
            current->second.term() > latestRecord->second.term()) {
            latestRecord = current;
        }
    }

    if (latestRecord == thesisRecords_.end()) {
        throw NotRegisteredException(courseCode);
    }

    Grade grade = Grade::create(symbol);

    // Do not call course.validateGrade here: conversion accepts letter grades.
    // ThesisRecord validates whether its current state permits conversion.
    latestRecord->second.convertToFinalGrade(grade);
}

int Registrar::calculateGpaHundredths(
    const std::string& studentId
) const {
    const Student& student = getStudent(studentId);

    long long weightedPoints = 0;
    long long totalCredits = 0;

    // Count only the latest finalized attempt for each course.
    std::map<std::string, const Attempt*> countingAttempts;

    for (const Attempt& attempt : student.attempts()) {
        if (!attempt.grade().isFinal()) {
            continue;
        }

        auto it = countingAttempts.find(attempt.courseCode());

        if (it == countingAttempts.end() ||
            attempt.attemptNumber() > it->second->attemptNumber()) {
            countingAttempts[attempt.courseCode()] = &attempt;
        }
    }

    for (const auto& entry : countingAttempts) {
        const Attempt& attempt = *entry.second;
        const Course& course = getCourse(attempt.courseCode());
        const Grade& grade = attempt.grade();

        if (!course.countsInGpa(grade)) {
            continue;
        }

        weightedPoints +=
            static_cast<long long>(course.credits()) *
            grade.pointsHundredths();

        totalCredits += course.credits();
    }

    // Include only finalized thesis grades.
    for (const auto& entry : thesisRecords_) {
        const std::string prefix = studentId + "|";

        if (entry.first.compare(0, prefix.size(), prefix) != 0) {
            continue;
        }

        const ThesisRecord& record = entry.second;

        if (record.state() != ThesisState::FINAL ||
            !record.finalGrade().has_value()) {
            continue;
        }

        const Course& course = getCourse(record.courseCode());
        const Grade& grade = *record.finalGrade();

        if (!course.countsInGpa(grade)) {
            continue;
        }

        weightedPoints +=
            static_cast<long long>(course.credits()) *
            grade.pointsHundredths();

        totalCredits += course.credits();
    }

    if (totalCredits == 0) {
        throw NoGpaDataException();
    }

    // Round to the nearest hundredth using half-up rounding.
    return static_cast<int>(
        (2 * weightedPoints + totalCredits) /
        (2 * totalCredits)
    );
}

std::string Registrar::calculateAverage(
    const std::string& studentId
) const {
    const int gpa = calculateGpaHundredths(studentId);

    if (gpa >= 950) return "A";
    if (gpa >= 850) return "A-";
    if (gpa >= 750) return "B";
    if (gpa >= 650) return "B-";
    if (gpa >= 550) return "C";
    if (gpa >= 450) return "C-";
    if (gpa >= 350) return "D";
    if (gpa >= 100) return "E";

    return "F";
}

std::vector<std::string> Registrar::graduationFailures(
    const std::string& studentId
) const {
    const Student& student = getStudent(studentId);

    std::vector<std::string> failures;

    int earnedCredits = 0;
    int electiveCredits = 0;
    bool allCorePassed = true;
    bool allMandatorySuPassed = true;
    bool thesisRequirementMet = false;
    bool hasPendingGrade = false;

    // Find the latest finalized attempt for each course.
    std::map<std::string, const Attempt*> latestAttempts;

    for (const Attempt& attempt : student.attempts()) {
        const std::string& code = attempt.courseCode();

        auto it = latestAttempts.find(code);

        if (it == latestAttempts.end() ||
            attempt.attemptNumber() > it->second->attemptNumber()) {
            latestAttempts[code] = &attempt;
        }
    }

    // R7: Only the latest attempt's incomplete grade is pending.
    for (const auto& entry : latestAttempts) {
        if (entry.second->grade().symbol() == "I") {
            hasPendingGrade = true;
            break;
        }
    }

    // Check course credits and category requirements.
    for (const auto& entry : courses_) {
        const std::string& code = entry.first;
        const Course& course = *entry.second;

        auto it = latestAttempts.find(code);

        if (it == latestAttempts.end()) {
            if (course.category() == "CORE") {
                allCorePassed = false;
            }

            if (course.category() == "MANDATORY_SU") {
                allMandatorySuPassed = false;
            }

            continue;
        }

        const Grade& grade = it->second->grade();
        const bool passed = course.earnsCredits(grade);

        if (course.category() == "CORE" && !passed) {
            allCorePassed = false;
        }

        if (course.category() == "MANDATORY_SU" &&
            grade.symbol() != "S") {
            allMandatorySuPassed = false;
        }

        if (passed) {
            earnedCredits += course.credits();

            if (course.category() == "ELECTIVE") {
                electiveCredits += course.credits();
            }
        }
    }

    // Check finalized thesis records.
    for (const auto& entry : thesisRecords_) {
        const std::string prefix = studentId + "|";

        if (entry.first.compare(0, prefix.size(), prefix) != 0) {
            continue;
        }

        const ThesisRecord& record = entry.second;

        if (record.state() != ThesisState::FINAL ||
            !record.finalGrade().has_value()) {
            continue;
        }

        const Grade& grade = *record.finalGrade();
        const Course& course = getCourse(record.courseCode());

        if (course.earnsCredits(grade)) {
            earnedCredits += course.credits();
        }

        if (grade.isPass() &&
            grade.pointsHundredths() >=
                Grade::create(rules_.thesisMinimumGrade())
                    .pointsHundredths()) {
            thesisRequirementMet = true;
        }
    }

    // R1: Total earned credits.
    if (earnedCredits < rules_.totalCredits()) {
        failures.push_back("R1");
    }

    // R2: All CORE courses must be passed.
    if (!allCorePassed) {
        failures.push_back("R2");
    }

    // R3: Minimum elective credits.
    if (electiveCredits < rules_.electiveCredits()) {
        failures.push_back("R3");
    }

    // R4: All MANDATORY_SU courses must have grade S.
    if (!allMandatorySuPassed) {
        failures.push_back("R4");
    }

    // R5: Thesis requirement.
    if (!thesisRequirementMet) {
        failures.push_back("R5");
    }

    // R6: Minimum GPA.
    try {
        if (calculateGpaHundredths(studentId) <
            rules_.minimumGpaHundredths()) {
            failures.push_back("R6");
        }
    } catch (const NoGpaDataException&) {
        failures.push_back("R6");
    }

    // R7: No pending or incomplete regular/SU grades.
    if (hasPendingGrade) {
        failures.push_back("R7");
    }

    return failures;
}

std::string Registrar::generateTranscript(
    const std::string& studentId
) const {
    const Student& student = getStudent(studentId);

    std::ostringstream output;
    output << "TRANSCRIPT " << studentId;

    // Regular and SU course attempts.
    for (const Attempt& attempt : student.attempts()) {
        output << " " << attempt.courseCode()
               << "[" << attempt.term()
               << ",#" << attempt.attemptNumber()
               << "]=" << attempt.grade().symbol();
    }

    // Thesis records.
    const std::string prefix = studentId + "|";

    for (const auto& entry : thesisRecords_) {
        if (entry.first.compare(0, prefix.size(), prefix) != 0) {
            continue;
        }

        const ThesisRecord& record = entry.second;

        output << " " << record.courseCode()
               << "[" << record.term() << "]="
               << "STATE:";

        switch (record.state()) {
            case ThesisState::REGISTERED:
                output << "REGISTERED";
                break;
            case ThesisState::IN_PROGRESS:
                output << "IN_PROGRESS";
                break;
            case ThesisState::AT_RISK:
                output << "AT_RISK";
                break;
            case ThesisState::TERMINATED:
                output << "TERMINATED";
                break;
            case ThesisState::FINAL:
                output << "FINAL";
                break;
        }

        output << ",PROGRESS:";

        const std::vector<Grade>& grades = record.progressGrades();

        for (std::size_t i = 0; i < grades.size(); ++i) {
            if (i > 0) {
                output << ",";
            }
            output << grades[i].symbol();
        }

        if (record.finalGrade().has_value()) {
            output << ",FINAL_GRADE:"
                   << record.finalGrade()->symbol();
        }
    }

    return output.str();
}

} // namespace acaderp
