// group-63
#ifndef GRP_63_COURSE_MATERIAL_H
#define GRP_63_COURSE_MATERIAL_H

#include <memory>

namespace acaderp {

class CourseMaterial {
public:
    virtual ~CourseMaterial() = default;

    virtual std::unique_ptr<CourseMaterial> clone() const = 0;
};

}  // namespace acaderp

#endif