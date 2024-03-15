//
// Created by talentstream on 2024/3/15.
//


#include <doctest.h>

#include <core/propertylist.h>

TEST_CASE("test property list") {
    USING_NAMESPACE
    PropertyList list;
    list.SetInt("int", 1);
    CHECK(list.GetInt("default", {}) == 0);
    CHECK(list.GetInt("int", {}) == 1);
}
