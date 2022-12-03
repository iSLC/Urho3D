#include "doctest/doctest_fwd.h"

int main(int argc, char** argv) {
    doctest::Context context;

    context.applyCommandLine(argc, argv);

    int res = context.run(); // run

    if(context.shouldExit())
        return res;
}
