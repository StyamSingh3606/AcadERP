// group-63
#ifndef GRP_63_GRADE_H
#define GRP_63_GRADE_H

#include <string>

namespace acaderp {

class Grade {
private:
    std::string symbol_;

    explicit Grade(const std::string& symbol);

public:
    static Grade create(const std::string& symbol);

    const std::string& symbol() const noexcept;
    int pointsHundredths() const;

    bool isPass() const noexcept;
    bool isFinal() const noexcept;

};

} // namespace acaderp

#endif// group-63
