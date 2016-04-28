#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>

#include "../lib/hashids/hashids.h"

#include "ggenygraph.h"
#include "ggenyio.h"
#include "ggenyrand.h"

const char *DIR = "export/";
const int FILE_NAME_META = 0;
const int FILE_NAME_OPL = 1;

char* get_file_name(Graph *graph, int is_opl)
{
    // for hashids
    struct hashids_t *hashids;
    char hash[512];
    unsigned long long numbers[1];

    char *file_name = (char *)malloc(255);

    // generate the base of the file name
    sprintf(file_name, "v%d_a%d_r%d_b%d",
        graph->nb_vertices,
        graph->nb_arcs,
        graph->nb_requests,
        graph->nb_blockages
    );

    if (graph->per_multiarcs > 0) {
        char cat[4];
        sprintf(cat, "_%d", graph->per_multiarcs);
        strcat(file_name, cat);
    }

    if (is_opl) {
        strcat(file_name, "_OPL");
    }

    /*
    // force the number to be the current time in second
    gettimeofday(&g_tval, NULL);
    numbers[0] = g_tval.tv_sec;

    // generate an unique id by hashids
    hashids = hashids_init3(file_name, 0, HASHIDS_DEFAULT_ALPHABET);
    hashids_encode(hashids, hash, sizeof(numbers) / sizeof(unsigned long long), numbers);

    strcat(file_name, "_");
    strcat(file_name, hash);
    strcat(file_name, ".dat");

    hashids_free(hashids);
    */

    strcat(file_name, ".dat");
    return file_name;
}

FILE* get_file(char *file_name)
{
    FILE *file;
    char *file_path = (char *)malloc(strlen(DIR));
    
    // construct a path to the directory of file storage
    // to avoid store the output directly in root dir.
    memcpy(file_path, DIR, strlen(DIR) + 1);
    strcat(file_path, file_name);
    file = fopen(file_path, "w+");
    free(file_path);

    return file;
}

bool output_meta(Graph *graph)
{
    int i;

    // holders
    Vertex *v;
    Arc *a;
    Request *r;
    Blockage *b;

    char *file_name = get_file_name(graph, FILE_NAME_META);
    FILE *file = get_file(file_name);

    if (!file) {
        printf("[ERR] failed to open file.\n");
        return false;
    } else {
        fprintf(file, "INSTANCE_NAME %s\n", file_name);
        fprintf(file, "NB_VERTICES %d\n", graph->nb_vertices);
        fprintf(file, "NB_ARCS %d\n", graph->nb_arcs);
        fprintf(file, "NB_REQUESTS %d\n", graph->nb_requests);
        fprintf(file, "NB_BLOCKAGES %d\n", graph->nb_blockages);
        if (graph->per_multiarcs > 0) {
            fprintf(file, "PER_MULTIARCS %d\n", graph->per_multiarcs);
        }
        fprintf(file, "\n");

        fprintf(file, "VERTICES\n");        
        for (i = 0; i < graph->nb_vertices; i++) {
            v = graph->vertices[i];
            fprintf(file, "%d %d %d\n",
                v->id + 1,
                v->x,
                v->y
            );
        }
        fprintf(file, "\n");

        fprintf(file, "ARCS\n"); 
        for (i = 0; i < graph->nb_arcs; i++) {
            a = graph->arcs[i];
            fprintf(file, "%d %d %d\n",
                a->source + 1,
                a->target + 1,
                a->cost
            );
        }
        fprintf(file, "\n");

        fprintf(file, "REQUESTS\n");
        for (i = 0; i < graph->nb_requests; i++) {
            r = graph->requests[i];
            fprintf(file, "%d %d %d %d\n",
                r->source,
                r->target,
                r->quantity,
                r->type
            );
        }
        fprintf(file, "\n");

        fprintf(file, "BLOCKAGES\n");
        for (i = 0; i < graph->nb_blockages; i++) {
            b = graph->blockages[i];
            fprintf(file, "%d %d\n",
                b->source + 1,
                b->target + 1
                //b->earliest_start,
                //b->latest_end,
                //b->duration
            );
        }
        fprintf(file, "\n");

        fprintf(file, "END\n");

        fclose(file);
        return true;
    }
}

bool output_opl(Graph *graph)
{
    int i;

    // holders
    Vertex *v;
    Arc *a;
    Request *r;
    Blockage *b;

    char *file_name = get_file_name(graph, FILE_NAME_OPL);
    FILE *file = get_file(file_name);

    if (!file) {
        printf("[ERR] failed to open file.\n");
        return false;
    } else {
        fprintf(file, "n = %d;\n", graph->nb_vertices);
        fprintf(file, "m = %d;\n", graph->nb_arcs);
        fprintf(file, "\n");

        fprintf(file, "arcs = {\n"); 
        for (i = 0; i < graph->nb_arcs; i++) {
            a = graph->arcs[i];
            fprintf(file, "<%d, %d, %d>",
                a->source + 1,
                a->target + 1,
                a->cost
            );
            if (i != graph->nb_arcs - 1) {
                fprintf(file, ",");
            }
            fprintf(file, "\n");
        }
        fprintf(file, "};\n\n");

        fprintf(file, "requests = {\n");
        for (i = 0; i < graph->nb_requests; i++) {
            r = graph->requests[i];
            fprintf(file, "<%d, %d, %d, %d>",
                r->source,
                r->target,
                r->quantity,
                r->type
            );
            if (i != graph->nb_requests - 1) {
                fprintf(file, ",");
            }
            fprintf(file, "\n");
        }
        fprintf(file, "};\n\n");

        fprintf(file, "blockages = {\n");
        for (i = 0; i < graph->nb_blockages; i++) {
            b = graph->blockages[i];
            fprintf(file, "<%d, %d>",
                b->source + 1,
                b->target + 1

                // TODO: blockage timescale for next model version
                //b->earliest_start,
                //b->latest_end,
                //b->duration
            );
            if (i != graph->nb_blockages - 1) {
                fprintf(file, ",");
            }
            fprintf(file, "\n");
        }
        fprintf(file, "};\n\n");

        fprintf(file, "// END\n");

        fclose(file);
        return true;
    }
}

Graph* input_meta(char *file_name)
{
    int i;
    const double epsilon = 1e-3;

    Graph *graph;
    int is_grid;
    
    // holders of metadata
    char name[255];
    int nb_vertices;
    int nb_arcs;
    int nb_requests;
    int nb_blockages;
    int size;

    // holders of elements
    Vertex *v;
    Arc *a;
    Request *r;
    Blockage *b;

    FILE *file = fopen(file_name, "r");

    if (!file) {
        printf("[ERR] failed to open file.\n");
        return NULL;
    }

    fscanf(file, "INSTANCE_NAME\t%s\n", name);
    fscanf(file, "NB_VERTICES%d\n", &nb_vertices);
    fscanf(file, "NB_ARCS%d\n", &nb_arcs);
    fscanf(file, "NB_REQUESTS%d\n", &nb_requests);
    fscanf(file, "NB_BLOCKAGES%d\n", &nb_blockages);
    
    if (0 == nb_vertices || 0 == nb_arcs) {
        printf("[ERR] empty graph.\n");
        return NULL;
    }

    size = (int) sqrt(nb_vertices);
    is_grid = abs(pow(size, 2) - nb_vertices) < epsilon;
    if (!is_grid) {
        // for now, if a graph is not square grid, we cannot do init.
        printf("[ERR] the graph is not square grid. (size=%d, nbv=%d)\n", size, nb_vertices);
        return NULL;
    }

    graph = init_grid_graph(size, nb_requests, nb_blockages);
    graph->nb_arcs = nb_arcs;

    // pass
    fscanf(file, "\n");
    fscanf(file, "VERTICES\n");

    for (i = 0; i < nb_vertices; i++) {
        v = graph->vertices[i];
        fscanf(file, "%d\t%d\t%d\n", &v->id, &v->x, &v->y);
        v->id -= 1;
        v->nb_adjacencies = 0;
    }

    // pass
    fscanf(file, "\n");
    fscanf(file, "ARCS\n");

    for (i = 0; i < nb_arcs; i++) {
        a = graph->arcs[i];
        int source, target, cost;
        fscanf(file, "%d\t%d\t%d\n", &source, &target, &cost);
        set_arc_attr(graph, i, source-1, target-1, cost);
    }

    // pass
    fscanf(file, "\n");
    fscanf(file, "REQUESTS\n");

    // TODO

    // pass
    fscanf(file, "\n");
    fscanf(file, "BLOCKAGES\n");
    for (i = 0; i < nb_blockages; i++) {
        b = graph->blockages[i];
        int source, target;
        fscanf(file, "%d\t%d\n", &source, &target);
        b->id = i;
        b->source = source - 1;
        b->target = target - 1;
    }

    fclose(file);

    return graph;
}
