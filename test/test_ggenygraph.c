#include <stdio.h>

#include "../lib/cutest/CuTest.h"
#include "../src/ggenygraph.h"

void test_init_grid_graph(CuTest *tc)
{
    Graph *graph = init_grid_graph(3, 0, 1);

    int expected = graph->nb_vertices == 9 &&
        graph->nb_arcs == 12 &&
        graph->nb_requests == 0 &&
        graph->nb_blockages == 1;

    CuAssertTrue(tc, expected);
}

CuSuite* test_ggenygraph_suite()
{
    CuSuite* suite = CuSuiteNew();
    
    SUITE_ADD_TEST(suite, test_init_grid_graph);

    return suite;
}
