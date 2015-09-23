#include <stdlib.h>

typedef struct Blockage {
    int source;
    int target;
    long earliest_start;
    long latest_end;
    long duration;
} Blockage;

typedef struct Request {
    int source;
    int target;
    int quantity;
    int type;
} Request;

typedef struct Arc {
    int source;
    int target;
    int cost;
    int capacity;
    int flow;
} Arc;

typedef struct Vertex {
    int x;
    int y;
    Arc *adjacent_arcs;
} Vertex;

typedef struct Graph {
    int size;

    int nb_vertices;
    int nb_arcs;
    int nb_requests;
    int nb_blockages;

    Vertex **vertices;
    Arc **arcs;
    Request **requests;
    Blockage **blockages;
} Graph;

Graph* init_grid_graph(int size, int nb_requests, int nb_blockages)
{
    int i;

    Graph *g = (Graph *)malloc(sizeof(Graph));

    g->size = size;

    g->nb_vertices = size * size;
    g->nb_arcs = size * (size - 1) * 2;
    g->nb_requests = nb_requests;
    g->nb_blockages = nb_blockages;

    g->vertices = (Vertex **)malloc(sizeof(Vertex *) * g->nb_vertices);
    g->arcs = (Arc **)malloc(sizeof(Arc *) * g->nb_arcs);
    g->requests = (Request **)malloc(sizeof(Request *) * nb_requests);
    g->blockages = (Blockage **)malloc(sizeof(Blockage *) * nb_blockages);

    for (i = 0; i < g->nb_vertices; i++) {
        g->vertices[i] = (Vertex *)malloc(sizeof(Vertex));
    }
    for (i = 0; i < g->nb_arcs; i++) {
        g->arcs[i] = (Arc *)malloc(sizeof(Arc));
    }
    for (i = 0; i < nb_requests; i++) {
        g->requests[i] = (Request *)malloc(sizeof(Request));
    }
    for (i = 0; i < nb_blockages; i++) {
        g->blockages[i] = (Blockage *)malloc(sizeof(Blockage));
    }

    return g;
}

void compute_grid_vertices(Graph *graph, int o_x, int o_y, int unit_cost)
{
    int i, j;

    // for computing vertex index
    int id;

    // i iterate index of columns
    for (i = 0; i < graph->size; i++) {
        // j iterate index of lines on every column
        for (j = 0; j < graph->size; j++) {
            id = i * graph->size + j;
            graph->vertices[id]->x = o_x + i * unit_cost;
            graph->vertices[id]->y = o_y + j * unit_cost;
        }
    }
}
