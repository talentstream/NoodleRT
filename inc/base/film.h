//
// Created by 44772 on 2024/2/11.
//

#pragma once

#include "core/object.h"

NAMESPACE_BEGIN

class Film : public Object {
public:
    explicit Film(const PropertyList &propertyList);

    virtual ~Film() = default;

    [[nodiscard]] EClassType GetClassType() const override {
        return EClassType::EFilm;
    }

private:

};

NAMESPACE_END