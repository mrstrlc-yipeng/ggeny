#include <stdio.h>
#include <stdint.h>

#include "ggenygraph.h"
#include "ggenyio.h"

void test_ggenygraph(void);
void test_ggenyio(void);

int main(void) {
    test_ggenygraph();
    test_ggenyio();
    return 0;
}

void test_ggenygraph(void) {
    int i, j;
    Graph *graph = compute_grid_graph(3, 1, 0, 0);

    printf("Graph %dx%d: %d vertices & %d arcs\n\n", graph->size, graph->size, graph->nb_vertices, graph->nb_arcs);
    printf("\n");

    printf("1. Vertex coordinates\n");
    for (i = 0; i < graph->nb_vertices; i++) {
        printf("\tv[%d] = (%d,%d)\n", graph->vertices[i]->id+1, graph->vertices[i]->x, graph->vertices[i]->y);
    }
    printf("\n");

    printf("2. Vertex adjacencies\n");
    for (i = 0; i < graph->nb_vertices; i++) {
        printf("\tv[%d] ajc(%d):\n", i+1, graph->vertices[i]->nb_adjacencies);
        for (j = 0; j < graph->vertices[i]->nb_adjacencies; j++) {
            printf("\t\t(%d-%d)\n",
                graph->arcs[graph->vertices[i]->adjacent_arcs[j]]->source+1,
                graph->arcs[graph->vertices[i]->adjacent_arcs[j]]->target+1
            );
        }
    }
    printf("\n");

    printf("3. Arcs\n");
    for (i = 0; i < graph->nb_arcs; i++) {
        printf("a[%d] (%d-%d), c = %d\n",
            graph->arcs[i]->id,
            graph->arcs[i]->source+1,
            graph->arcs[i]->target+1,
            graph->arcs[i]->cost);
    }

    free_graph(graph);
}

void test_ggenyio(void) {
    Graph *graph = compute_grid_graph(3, 1, 0, 0);

    if (output_meta(graph)) {
        printf("META output file ok\n");
    } else {
        printf("META output file failed \n");
    }

    if (output_opl(graph)) {
        printf("OPL output file ok\n");
    } else {
        printf("OPL output file failed \n");
    }
    
    free_graph(graph);
}
