#include <stdlib.h>

#include "xorshift.c"

const int DEFAULT_NB_ADJACENCY = 4;

Graph* init_grid_graph(int size, int nb_requests, int nb_blockages)
{
    Graph *g = (Graph *)malloc(sizeof(Graph));

    g->size = size;

    g->nb_vertices = size * size;
    g->nb_arcs = size * (size - 1) * 2;
    g->nb_requests = nb_requests;
    g->nb_blockages = nb_blockages;

    g->vertices = (Vertex *)malloc(sizeof(Vertex) * g->nb_vertices);
    g->arcs = (Arc *)malloc(sizeof(Arc) * g->nb_arcs);
    g->requests = (Request *)malloc(sizeof(Request) * nb_requests);
    g->blockages = (Blockage *)malloc(sizeof(Blockage) * nb_blockages);

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
            graph->vertices[id].x = o_x + i * unit_cost;
            graph->vertices[id].y = o_y + j * unit_cost;
        }
    }
}

int* get_grid_adjacency(int size, int v_id)
{
    // normally, the vertices apart from the boundary
    // have 4 adjacencies (up, down, left, right).
    // So the declaration goes with 4 adjacency while 
    // if the concerning vertex is on boundary (or 
    // even on corners), the last 1 or 2 adjacencies 
    // will be negative, which indicates the inexistance.
    // Be attention to validate every adjacencies when
    // using them.
    int *adjacency = (int *)malloc(sizeof(int) * DEFAULT_NB_ADJACENCY);

    // compute column and row indices of the vertex
    int row = v_id % size;
    int col = (v_id - row) / size;

    bool is_bound = 0 == row || 0 == col || (size-1) == row || (size-1) == col;
    bool is_corner = (0 == row && 0 == col) ||
        (0 == row && (size-1) == col) ||
        ((size-1) == row && 0 == col) ||
        ((size-1) == row && (size-1) == col);

    int i;

    for (i = 0; i < DEFAULT_NB_ADJACENCY; i++) {
        adjacency[i] = -1;
    }

    if (!is_bound) {
        // left-up-right-down
        adjacency[0] = v_id - size;
        adjacency[1] = v_id + 1;
        adjacency[2] = v_id + size;
        adjacency[3] = v_id - 1;
    } else if (is_corner) {
        if (0 == row) {
            // up adjacency is ensured
            adjacency[0] = v_id + 1;
        } else {
            // down adjacency is ensured
            adjacency[0] = v_id - 1;
        }

        if (0 == col) {
            // right adjacency is ensured
            adjacency[1] = v_id + size;
        } else {
            // left adjacency is ensured
            adjacency[1] = v_id - size;
        }
    } else {
        if (0 == row) {
            // down boundary
            // left-up-right-(-1)
            adjacency[0] = v_id - size;
            adjacency[1] = v_id + 1;
            adjacency[2] = v_id + size;
        } else if ((size-1) == row) {
            // up boundary
            // left-right-down-(-1)
            adjacency[0] = v_id - size;
            adjacency[1] = v_id + size;
            adjacency[2] = v_id - 1;
        } else if (0 == col) {
            // left boundary
            // up-right-down-(-1)
            adjacency[0] = v_id + 1;
            adjacency[1] = v_id + size;
            adjacency[2] = v_id - 1;
        } else {
            // right boundary
            // left-up-down
            adjacency[0] = v_id - size;
            adjacency[1] = v_id + 1;
            adjacency[2] = v_id - 1;
        }
    }
}

void set_grid_orientation_1(int size, int v_id, int *marks)
{
    int i;
    int *adjacency = get_grid_adjacency(size, v_id);

    marks[v_id] = 1;
    for (i = 0; i < DEFAULT_NB_ADJACENCY; i++) {
        if (adjacency[i] >= 0 && !marks[adjacency[i]]) {
            set_grid_orientation_1(size, adjacency[i], marks);
        }
    }
}

void set_grid_orientation_2(int nba, int **arcs, int *marks)
{
    int i;
    int source, target;

    for (i = 0; i < nba; i++) {
        source = arcs[i][0] - 1;
        target = arcs[i][1] - 1;
    }
}

int** get_grid_arcs(int size, int unit_cost)
{
    int i, j;

    // number of arcs=2s(s-1)
    // each column or line has (size-1) arcs
    int nba = size * (size - 1) * 2;

    // for computing indices
    int source_id;
    int arc_id;

    // vertex marks for orientation (DFS)
    int *marks = (int *)malloc(sizeof(int) * size * size);

    // arcs[id]=[source,target,cost]
    int **arcs = (int **)malloc(sizeof(int *) * nba);
    for (i = 0; i < nba; i++) {
        arcs[i] = (int *)malloc(sizeof(int) * 3);
    }

    // initialize arcs father->son
    for (i = 0; i < size - 1; i++) {
        for (j = 0; j < size - 1; j++) {
            source_id = i * size + j;
            arc_id = i * (size - 1) + j;
            arcs[id][0] = source_id + 1;
            arcs[id][1] = (source_id + 1) + 1;
            arcs[id][2] = unit_cost;
        }
    }

    // manage orientation for strong connectivity
}

int** get_requests()
{}

int** get_blockages()
{}
