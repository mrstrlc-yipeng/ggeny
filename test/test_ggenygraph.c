#include <stdio.h>
#include <stdlib.h>

#include "../lib/cutest/CuTest.h"
#include "../src/ggenygraph.h"


/* Test of utility functions */

void test_ensure_ajc_alloc(CuTest *tc)
{
    int expected = 0;

    Vertex *v = (Vertex *)malloc(sizeof(Vertex));
    v->id = 0;
    v->x = 0;
    v->y = 0;
    v->nb_adjacencies = 0;

    expected = NULL == v->adjacent_arcs;
    CuAssertTrue(tc, expected);

    ensure_ajc_alloc(v);
    expected = 1 == sizeof(v->adjacent_arcs) / sizeof(int);
    CuAssertTrue(tc, expected);
}

void test_set_arc_attr(CuTest *tc)
{
    int expected = 0;

    Graph *graph = init_grid_graph(3, 0, 1);
    
    set_arc_attr(graph, 0, 0, 1, 1);
    Arc *arc = graph->arcs[0];
    Vertex *source = graph->vertices[0];
    Vertex *target = graph->vertices[1];

    expected = 0 == arc->source &&
        1 == arc->target &&
        1 == arc->cost;
    CuAssertTrue(tc, expected);

    expected = 1 == source->nb_adjacencies &&
        0 == source->adjacent_arcs[0];
    CuAssertTrue(tc, expected);

    expected = 1 == target->nb_adjacencies &&
        0 == target->adjacent_arcs[0];
    CuAssertTrue(tc, expected);
}

void test_reverse_direction(CuTest *tc)
{
    int expected = 0;

    Arc *arc = (Arc *)malloc(sizeof(Arc));
    arc->source = 0;
    arc->target = 1;
    arc->cost = 1;

    reverse_direction(arc);
    expected = 1 == arc->source && 0 == arc->target;
    CuAssertTrue(tc, expected);
}

void test_set_grid_direction_1(CuTest *tc)
{}

void test_set_grid_direction_2(CuTest *tc)
{}

void test_verify_nb_blockage(CuTest *tc)
{
    int expected = 0;
    Graph *graph = init_grid_graph(3, 0, 5);

    expected = 5 == graph->nb_blockages;
    CuAssertTrue(tc, expected);

    verify_nb_blockage(graph);
    expected = 4 == graph->nb_blockages;
    CuAssertTrue(tc, expected);
}

void test_is_arc_at_corner(CuTest *tc)
{
    int expected = 0;
    int i;
    Graph *graph = init_grid_graph(2, 0, 0);
    
    set_arc_attr(graph, 0, 0, 1, 1);
    set_arc_attr(graph, 1, 0, 2, 1);
    set_arc_attr(graph, 2, 1, 3, 1);
    set_arc_attr(graph, 3, 2, 3, 1);

    for (i = 0; i < 4; i++) {
        expected = is_arc_at_corner(graph, graph->arcs[i]);
        CuAssertTrue(tc, expected);
    }
}

void test_set_blockage_attr(CuTest *tc)
{
    int expected = 0;
    Graph *graph = init_grid_graph(3, 0, 1);

    set_arc_attr(graph, 0, 0, 1, 1);
    set_blockage_attr(graph, 0, graph->arcs[0]);
    expected = 0 == graph->blockages[0]->id &&
        0 == graph->blockages[0]->source &&
        1 == graph->blockages[0]->target;
    CuAssertTrue(tc, expected);
}


/* Test of intermediate functions */

void test_init_grid_graph(CuTest *tc)
{
    Graph *graph = init_grid_graph(3, 0, 1);

    int expected = graph->nb_vertices == 9 &&
        graph->nb_arcs == 12 &&
        graph->nb_requests == 0 &&
        graph->nb_blockages == 1;

    CuAssertTrue(tc, expected);
}

void test_compute_grid_vertices(CuTest *tc)
{}

void test_compute_grid_arcs(CuTest *tc)
{}

void test_compute_grid_directions(CuTest *tc)
{}

void test_compute_requests(CuTest *tc)
{}

void test_compute_blockages(CuTest *tc)
{}


/* Test of feature functions */

void test_compute_grid_graph(CuTest *tc)
{}

void test_patch_multiarcs_2(CuTest *tc)
{}

void test_patch_multiarcs(CuTest *tc)
{}

void test_free_graph(CuTest *tc)
{}


/* Test suite */
CuSuite* test_ggenygraph_suite()
{
    CuSuite* suite = CuSuiteNew();
    
    // Test of utility functions
    SUITE_ADD_TEST(suite, test_ensure_ajc_alloc);
    SUITE_ADD_TEST(suite, test_set_arc_attr);
    SUITE_ADD_TEST(suite, test_reverse_direction);
    SUITE_ADD_TEST(suite, test_set_grid_direction_1);
    SUITE_ADD_TEST(suite, test_set_grid_direction_2);
    SUITE_ADD_TEST(suite, test_verify_nb_blockage);
    SUITE_ADD_TEST(suite, test_is_arc_at_corner);
    SUITE_ADD_TEST(suite, test_set_blockage_attr);

    // Test of intermediate functions
    SUITE_ADD_TEST(suite, test_init_grid_graph);
    SUITE_ADD_TEST(suite, test_compute_grid_vertices);
    SUITE_ADD_TEST(suite, test_compute_grid_arcs);
    SUITE_ADD_TEST(suite, test_compute_grid_directions);
    SUITE_ADD_TEST(suite, test_compute_requests);
    SUITE_ADD_TEST(suite, test_compute_blockages);

    // Test of feature functions
    SUITE_ADD_TEST(suite, test_compute_grid_graph);
    SUITE_ADD_TEST(suite, test_patch_multiarcs_2);
    SUITE_ADD_TEST(suite, test_patch_multiarcs);
    SUITE_ADD_TEST(suite, test_free_graph);

    return suite;
}
