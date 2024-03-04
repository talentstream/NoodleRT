//
// Created by talentstream on 2024/3/4.
//

#include "base/shape.h"

NAMESPACE_BEGIN

class Mesh : public shape {
public:
    explicit Mesh(const PropertyList &propertyList) {

    }

private:
    Transform mObjectToWorld;
    std::string_view mFilename;
};

NAMESPACE_END