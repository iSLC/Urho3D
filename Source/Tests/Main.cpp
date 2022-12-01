#include "doctest/doctest_fwd.h"

TEST_CASE("Dummy case") {
    CHECK(true);
}

int main(int argc, char** argv) {
    doctest::Context context;

    context.applyCommandLine(argc, argv);

    int res = context.run(); // run

    if(context.shouldExit())
        return res;
}
