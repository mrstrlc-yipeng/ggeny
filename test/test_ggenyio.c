#include <stdio.h>
#include <string.h>

#include "../lib/cutest/CuTest.h"
#include "../src/ggenyio.h"

Graph *fake_graph = NULL;

void get_fake_graph()
{
    // Generate a graph for test with
    //   9 vertices, 12 arc, 0 requests and 1 blockage.
    fake_graph = compute_grid_graph(3, 1, 0, 1);
}

void test_get_file_name(CuTest *tc)
{
    if (fake_graph == NULL) {
        get_fake_graph();
    }

    char *actual = get_file_name(fake_graph, 0);
    char *expected = "v9_a12_r0_b1";
    CuAssertTrue(tc, strstr(actual, expected) != NULL);
}

CuSuite* test_ggenyio_suite()
{
    CuSuite* suite = CuSuiteNew();
    
    SUITE_ADD_TEST(suite, test_get_file_name);

    return suite;
}
