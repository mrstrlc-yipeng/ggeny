#include <stdio.h>

#include "../lib/cutest/CuTest.h"

CuSuite* test_ggenyio_suite();
CuSuite* test_ggenygraph_suite();

void RunAllTests(void) {
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    CuSuiteAddSuite(suite, test_ggenyio_suite());
    CuSuiteAddSuite(suite, test_ggenygraph_suite());

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
}

int main(void) {
    RunAllTests();
}
