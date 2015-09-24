#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "ggenygraph.h"

char* get_file_name(Graph *graph)
{
    char *file_name = (char *)malloc(255);

    sprintf(file_name, "out/v%d_a%d_r%d_b%d.dat",
        graph->nb_vertices,
        graph->nb_arcs,
        graph->nb_requests,
        graph->nb_blockages
    );

    return file_name;
}

bool output_meta(Graph *graph)
{
    int i;
    char *file_name;
    FILE *file;

    // holders
    Vertex *v;
    Arc *a;
    Request *r;
    Blockage *b;

    file_name = get_file_name(graph);
    file = fopen(file_name, "w+");

    if (!file) {
        printf("[ERR] failed to open file.\n");
        return false;
    } else {
        fprintf(file, "INSTANCE_NAME\t%s\n", file_name);
        fprintf(file, "NB_VERTICES\t\t%d\n", graph->nb_vertices);
        fprintf(file, "NB_EDGES\t\t%d\n", graph->nb_arcs);
        fprintf(file, "NB_REQUESTS\t\t%d\n", graph->nb_requests);
        fprintf(file, "NB_BLOCKAGES\t%d\n", graph->nb_blockages);
        fprintf(file, "\n");

        fprintf(file, "VERTICES\n");        
        for (i = 0; i < graph->nb_vertices; i++) {
            v = graph->vertices[i];
            fprintf(file, "%d\t%d\t%d\n", v->id + 1, v->x, v->y);
        }
        fprintf(file, "\n");

        fprintf(file, "ARCS\n"); 
        for (i = 0; i < graph->nb_arcs; i++) {
            a = graph->arcs[i];
            fprintf(file, "%d\t%d\t%d\n", a->source, a->target, a->cost);
        }
        fprintf(file, "\n");

        fprintf(file, "REQUESTS\n");
        for (i = 0; i < graph->nb_requests; i++) {
            r = graph->requests[i];
            fprintf(file, "%d\t%d\t%d\n",
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
            fprintf(file, "%d\t%d\t%d\n",
                b->source,
                b->target,
                b->earliest_start,
                b->latest_end,
                b->duration
            );
        }
        fprintf(file, "\n");

        fprintf(file, "END");

        fclose(file);
        return true;
    }
}

bool output_opl(Graph *graph)
{
    return false;
}
