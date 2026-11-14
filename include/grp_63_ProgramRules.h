// group-63

// group-63
#ifndef GRP_63_PROGRAMRULES_H
#define GRP_63_PROGRAMRULES_H

#include <string>

namespace acaderp {

class ProgramRules {
private:
    int totalCredits_;
    int electiveCredits_;
    int minimumGpaHundredths_;
    std::string thesisMinimumGrade_;
    int thesisMinimumCredits_;
    int thesisMinimumGpaHundredths_;
    int maximumLoad_;

public:
    ProgramRules();

    int totalCredits() const noexcept;
    int electiveCredits() const noexcept;
    int minimumGpaHundredths() const noexcept;
    const std::string& thesisMinimumGrade() const noexcept;
    int thesisMinimumCredits() const noexcept;
    int thesisMinimumGpaHundredths() const noexcept;
    int maximumLoad() const noexcept;

    void setTotalCredits(int value);
    void setElectiveCredits(int value);
    void setMinimumGpaHundredths(int value);
    void setThesisMinimumGrade(const std::string& value);
    void setThesisMinimumCredits(int value);
    void setThesisMinimumGpaHundredths(int value);
    void setMaximumLoad(int value);
};

} // namespace acaderp

#endif
