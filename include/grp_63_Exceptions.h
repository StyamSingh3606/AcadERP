// group-63

// group-63
#ifndef GRP_63_EXCEPTIONS_H
#define GRP_63_EXCEPTIONS_H

#include <stdexcept>
#include <string>

namespace acaderp {

class ErpException : public std::runtime_error {
private:
    std::string code_;

public:
    ErpException(const std::string& code, const std::string& message);
    const std::string& code() const noexcept;
};

class InputException : public ErpException {
public:
    InputException(const std::string& code, const std::string& message);
};

class ParseException : public InputException {
public:
    explicit ParseException(const std::string& message);
};

class EntityException : public ErpException {
public:
    EntityException(const std::string& code, const std::string& message);
};

class DuplicateEntityException : public EntityException {
public:
    explicit DuplicateEntityException(const std::string& id);
};

class UnknownStudentException : public EntityException {
public:
    explicit UnknownStudentException(const std::string& roll);
};

class UnknownCourseException : public EntityException {
public:
    explicit UnknownCourseException(const std::string& code);
};

class InvalidCourseException : public EntityException {
public:
    explicit InvalidCourseException(const std::string& message);
};

class GradeException : public ErpException {
public:
    GradeException(const std::string& code, const std::string& message);
};

class InvalidGradeSymbolException : public GradeException {
public:
    explicit InvalidGradeSymbolException(const std::string& symbol);
};

class GradeTypeMismatchException : public GradeException {
public:
    explicit GradeTypeMismatchException(const std::string& message);
};

class NotRegisteredException : public GradeException {
public:
    explicit NotRegisteredException(const std::string& message);
};

class GradeAlreadyFinalException : public GradeException {
public:
    explicit GradeAlreadyFinalException(const std::string& message);
};

class InvalidThesisTransitionException : public GradeException {
public:
    explicit InvalidThesisTransitionException(const std::string& message);
};

class ThesisTerminatedException : public GradeException {
public:
    explicit ThesisTerminatedException(const std::string& message);
};

class RegistrationException : public ErpException {
public:
    RegistrationException(const std::string& code, const std::string& message);
};

class AlreadyPassedException : public RegistrationException {
public:
    explicit AlreadyPassedException(const std::string& message);
};

class DuplicateRegistrationException : public RegistrationException {
public:
    explicit DuplicateRegistrationException(const std::string& message);
};

class PrerequisiteNotMetException : public RegistrationException {
public:
    explicit PrerequisiteNotMetException(const std::string& message);
};

class AntiRequisiteException : public RegistrationException {
public:
    explicit AntiRequisiteException(const std::string& message);
};

class CreditLimitExceededException : public RegistrationException {
public:
    explicit CreditLimitExceededException(const std::string& message);
};

class ThesisNotAllowedException : public RegistrationException {
public:
    explicit ThesisNotAllowedException(const std::string& message);
};

class ComputationException : public ErpException {
public:
    ComputationException(const std::string& code, const std::string& message);
};

class NoGpaDataException : public ComputationException {
public:
    NoGpaDataException();
};

} // namespace acaderp

#endif
