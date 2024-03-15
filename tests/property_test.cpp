//
// Created by talentstream on 2024/3/15.
//


#include <doctest.h>

#include <core/propertylist.h>

TEST_CASE("Test Int") {
    USING_NAMESPACE
    PropertyList list;
    list.SetInt("int", 1);
    CHECK(list.GetInt("default", {}) == 0);
    CHECK(list.GetInt("int", {}) == 1);
    list.SetInt("intf", 1.5);
    CHECK(list.GetInt("intf", {}) == 1);
}

TEST_CASE("TEST Float") {
    USING_NAMESPACE

}