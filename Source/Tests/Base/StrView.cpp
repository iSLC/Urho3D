#include <doctest/doctest_fwd.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <type_traits>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

#include <Urho3D/Base/StrView.h>

// Test StriView.
TEST_CASE("StrView")
{
    using namespace Urho3D;

    StrView def;
    StrView empty("");
    StrView ex_empty("", 0);
    StrView x("x");
    StrView ex_x("x", 1);
    StrView xy("xy");
    StrView ex_xy("xy", 2);
    StrView xyz("xyz");
    StrView ex_xyz("xyz", 3);

    CHECK_EQ(def.Size(), 0);
    CHECK_EQ(empty.Size(), 0);
    CHECK_EQ(ex_empty.Size(), 0);

    CHECK_EQ(x.Size(), 1);
    CHECK_EQ(ex_x.Size(), 1);
    CHECK_EQ(xy.Size(), 2);
    CHECK_EQ(ex_xy.Size(), 2);
    CHECK_EQ(xyz.Size(), 3);
    CHECK_EQ(ex_xyz.Size(), 3);
}
