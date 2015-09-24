#include <stdlib.h>

#include "ggenyrand.h"

#ifndef GGENY_GRAPH_H
#define GGENY_GRAPH_H

typedef struct Blockage {
    int id;
    int source;
    int target;
    long earliest_start;
    long latest_end;
    long duration;
} Blockage;

typedef struct Request {
    int id;
    int source;
    int target;
    int quantity;
    int type;
} Request;

typedef struct Arc {
    int id;
    int source;
    int target;
    int cost;
    int capacity;
    int flow;
} Arc;

typedef struct Vertex {
    int id;
    int x;
    int y;
    int nb_adjacencies;
    int *adjacent_arcs;
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


// random seed as global var
int g_default_seed = 4;


// function declarations
// major feature functions
Graph* compute_grid_graph(int size, int unit_cost, int nb_requests, int nb_blockages);
void free_graph(Graph *graph);

// utilitiy functions
void ensure_ajc_alloc(Vertex *v);
void set_arc_attr(Graph *graph, int arc_id, int source_id, int target_id, int cost);
void reverse_direction(Arc *arc);
void set_grid_direction_1(Graph *graph, Vertex *v, int *marks);
void set_grid_direction_2(Graph *graph, int *marks);

// internal feature functions
Graph *init_grid_graph(int size, int nb_requests, int nb_blockages);
void compute_grid_vertices(Graph *graph, int o_x, int o_y, int unit_cost);
void compute_grid_arcs(Graph *graph, int unit_cost);
void compute_grid_directions(Graph *graph);


Graph* compute_grid_graph(int size, int unit_cost, int nb_requests, int nb_blockages)
{
    int coor_max = (size - 1) * unit_cost;
    int origin_x = rand_ij(&g_default_seed, 0, coor_max);
    int origin_y = rand_ij(&g_default_seed, 0, coor_max);

    Graph *graph = init_grid_graph(size, nb_requests, nb_blockages);
    
    compute_grid_vertices(graph, origin_x, origin_y, unit_cost);
    compute_grid_arcs(graph, unit_cost);
    compute_grid_directions(graph);

    return graph;
}

Graph *init_grid_graph(int size, int nb_requests, int nb_blockages)
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
        g->vertices[i]->nb_adjacencies = 0;
        g->vertices[i]->adjacent_arcs = NULL;
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
            graph->vertices[id]->id = id;
            graph->vertices[id]->x = o_x + i * unit_cost;
            graph->vertices[id]->y = o_y + j * unit_cost;
        }
    }
}

void ensure_ajc_alloc(Vertex *v)
{
    if (0 == v->nb_adjacencies) {
        v->adjacent_arcs = (int *)malloc(sizeof(int));
    } else {
        v->adjacent_arcs = (int *)realloc(
            v->adjacent_arcs,
            sizeof(int) * (v->nb_adjacencies + 1)
        );
    }

    if (!v->adjacent_arcs) {
        printf("[ERR] Vertex adjacency list allocation error at v[%d]", v->id);
    }
}

void set_arc_attr(Graph *graph, int arc_id, int source_id, int target_id, int cost)
{
    Vertex *source;
    Vertex *target;

    // indices are always in informatic way (0..n)
    graph->arcs[arc_id]->id = arc_id;
    graph->arcs[arc_id]->source = source_id;
    graph->arcs[arc_id]->target = target_id;
    graph->arcs[arc_id]->cost = cost;

    // TODO: integrate capacity and flow when necessary

    source = graph->vertices[source_id];
    target = graph->vertices[target_id];
    ensure_ajc_alloc(source);
    ensure_ajc_alloc(target);

    // associate adjacency for source and target
    source->adjacent_arcs[source->nb_adjacencies] = arc_id;
    target->adjacent_arcs[target->nb_adjacencies] = arc_id;
    source->nb_adjacencies++;
    target->nb_adjacencies++;
}

void compute_grid_arcs(Graph *graph, int unit_cost)
{
    int i, j;

    // for computing indices
    int source_id;
    int target_id;
    int arc_cnt = 0;

    // construct initial arcs of columns father->son (down->up)
    for (i = 0; i < graph->size; i++) {
        for (j = 0; j < graph->size - 1; j++) {
            source_id = i * graph->size + j;
            target_id = source_id + 1;
            set_arc_attr(graph, arc_cnt, source_id, target_id, unit_cost);

            arc_cnt++;
        }
    }

    // construct initial arcs of rows father->son (left->right)
    for (i = 0; i < graph->size - 1; i++) {
        for (j = 0; j < graph->size; j++) {
            source_id = i * graph->size + j;
            target_id = source_id + graph->size;
            set_arc_attr(graph, arc_cnt, source_id, target_id, unit_cost);

            arc_cnt++;
        }
    }
}

void reverse_direction(Arc *arc)
{
    int tmp;

    tmp = arc->source;
    arc->source = arc->target;
    arc->target = tmp;
}

void set_grid_direction_1(Graph *graph, Vertex *v, int *marks)
{
    int i;

    Vertex *next_v;
    Arc *via;

    // this procedure marks the link arc from v to its adjacencies.
    // these marks serve to retain all non-visited arcs for further
    // orientation step(s)
    for (i = 0; i < v->nb_adjacencies; i++) {
        // next adjacency
        via = graph->arcs[v->adjacent_arcs[i]];
        next_v = via->source == v->id
            ? graph->vertices[via->target]
            : graph->vertices[via->source];

        if (!marks[via->id]) {
            marks[via->id] = 1;

            // direction father->son
            // the vertex `v` should be the source of via
            if (via->source != v->id) {
                reverse_direction(via);
            }

            set_grid_direction_1(graph, next_v, marks);
        }
    }
}

void set_grid_direction_2(Graph *graph, int *marks)
{
    int i;

    Arc *arc;

    for (i = 0; i < graph->nb_arcs; i++) {
        if (!marks[i]) {
            arc = graph->arcs[i];

            // set direction descendant->ancestor
            reverse_direction(arc);
        }
    }
}

void compute_grid_directions(Graph *graph)
{
    int i;

    // random point to start with
    int origin = rand_ij(&g_default_seed, 0, graph->nb_vertices - 1);

    int *marks = (int *)malloc(sizeof(int) * graph->nb_arcs);
    for (i = 0; i < graph->nb_arcs; i++) {
        marks[i] = 0;
    }

    set_grid_direction_1(graph, graph->vertices[origin], marks);
    set_grid_direction_2(graph, marks);

    free(marks);
}

void free_graph(Graph *graph)
{
    int i;

    for (i = 0; i < graph->nb_vertices; i++) {
        free(graph->vertices[i]->adjacent_arcs);
        free(graph->vertices[i]);
    }
    for (i = 0; i < graph->nb_arcs; i++) {
        free(graph->arcs[i]);
    }
    for (i = 0; i < graph->nb_requests; i++) {
        free(graph->requests[i]);
    }
    for (i = 0; i < graph->nb_blockages; i++) {
        free(graph->blockages[i]);
    }
    free(graph->vertices);
    free(graph->arcs);
    free(graph->requests);
    free(graph->blockages);
    
    free(graph);
    graph = NULL;
}

#endif
