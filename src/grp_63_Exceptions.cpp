// group-63

// group-63
#include "grp_63_Exceptions.h"

namespace acaderp {

// Base exception
ErpException::ErpException(const std::string& code,
                           const std::string& message)
    : std::runtime_error(message), code_(code) {}

const std::string& ErpException::code() const noexcept {
    return code_;
}

// Input exceptions
InputException::InputException(const std::string& code,
                               const std::string& message)
    : ErpException(code, message) {}

ParseException::ParseException(const std::string& message)
    : InputException("E_PARSE", message) {}

// Entity exceptions
EntityException::EntityException(const std::string& code,
                                 const std::string& message)
    : ErpException(code, message) {}

DuplicateEntityException::DuplicateEntityException(const std::string& id)
    : EntityException("E_DUPLICATE",
                      "Entity already exists: " + id) {}

UnknownStudentException::UnknownStudentException(const std::string& roll)
    : EntityException("E_UNKNOWN_STUDENT",
                      "Student not found: " + roll) {}

UnknownCourseException::UnknownCourseException(const std::string& code)
    : EntityException("E_UNKNOWN_COURSE",
                      "Course not found: " + code) {}

InvalidCourseException::InvalidCourseException(const std::string& message)
    : EntityException("E_COURSE_DEF", message) {}

// Grade exceptions
GradeException::GradeException(const std::string& code,
                               const std::string& message)
    : ErpException(code, message) {}

InvalidGradeSymbolException::InvalidGradeSymbolException(
    const std::string& symbol)
    : GradeException("E_GRADE_SYMBOL",
                     "Invalid grade symbol: " + symbol) {}

GradeTypeMismatchException::GradeTypeMismatchException(
    const std::string& message)
    : GradeException("E_GRADE_TYPE", message) {}

NotRegisteredException::NotRegisteredException(
    const std::string& message)
    : GradeException("E_NOT_REGISTERED", message) {}

GradeAlreadyFinalException::GradeAlreadyFinalException(
    const std::string& message)
    : GradeException("E_GRADE_FINAL", message) {}

InvalidThesisTransitionException::InvalidThesisTransitionException(
    const std::string& message)
    : GradeException("E_THESIS_TRANSITION", message) {}

ThesisTerminatedException::ThesisTerminatedException(
    const std::string& message)
    : GradeException("E_THESIS_TERMINATED", message) {}

// Registration exceptions
RegistrationException::RegistrationException(
    const std::string& code, const std::string& message)
    : ErpException(code, message) {}

AlreadyPassedException::AlreadyPassedException(
    const std::string& message)
    : RegistrationException("E_ALREADY_PASSED", message) {}

DuplicateRegistrationException::DuplicateRegistrationException(
    const std::string& message)
    : RegistrationException("E_DUP_REG", message) {}

PrerequisiteNotMetException::PrerequisiteNotMetException(
    const std::string& message)
    : RegistrationException("E_PREREQ", message) {}

AntiRequisiteException::AntiRequisiteException(
    const std::string& message)
    : RegistrationException("E_ANTIREQ", message) {}

CreditLimitExceededException::CreditLimitExceededException(
    const std::string& message)
    : RegistrationException("E_CREDIT_LIMIT", message) {}

ThesisNotAllowedException::ThesisNotAllowedException(
    const std::string& message)
    : RegistrationException("E_THESIS_NOT_ALLOWED", message) {}

// Computation exceptions
ComputationException::ComputationException(
    const std::string& code, const std::string& message)
    : ErpException(code, message) {}

NoGpaDataException::NoGpaDataException()
    : ComputationException("E_NO_GPA",
                           "No GPA-eligible course data available") {}

} // namespace acaderp
