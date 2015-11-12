#include <sys/time.h>

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
    int per_multiarcs;

    Vertex **vertices;
    Arc **arcs;
    Request **requests;
    Blockage **blockages;
} Graph;


// random seed
extern struct timeval g_tval;
extern int g_default_seed;


// function declarations
// major feature functions
Graph* compute_grid_graph(int size, int unit_cost, int nb_requests, int nb_blockages);
void patch_multiarcs_2(Graph *graph, float percentage);
void patch_multiarcs(Graph *graph, float percentage, int nb_lanes_max);
void free_graph(Graph *graph);

// utilitiy functions
void ensure_ajc_alloc(Vertex *v);
void set_arc_attr(Graph *graph, int arc_id, int source_id, int target_id, int cost);
void reverse_direction(Arc *arc);
void set_grid_direction_1(Graph *graph, Vertex *v, int *marks);
void set_grid_direction_2(Graph *graph, int *marks);
void verify_nb_blockage(Graph *graph);
int is_arc_at_corner(Graph *graph, Arc *arc);
void set_blockage_attr(Graph *graph, int blockage_id, Arc *blocked_arc);

// internal feature functions
Graph *init_grid_graph(int size, int nb_requests, int nb_blockages);
void compute_grid_vertices(Graph *graph, int o_x, int o_y, int unit_cost);
void compute_grid_arcs(Graph *graph, int unit_cost);
void compute_grid_directions(Graph *graph);
void compute_requests(Graph *graph);
void compute_blockages(Graph *graph);

#endif
