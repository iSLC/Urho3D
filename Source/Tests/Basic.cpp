#include <doctest/doctest_fwd.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
// include std::headers here...
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

TEST_CASE("Dummy case") {
    int a = 5;
    int b = 5;
    REQUIRE(a == b);
}
