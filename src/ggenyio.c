#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>

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
    sprintf(file_name, "v%d_a%d_r%d_b%d_%s",
        graph->nb_vertices,
        graph->nb_arcs,
        graph->nb_requests,
        graph->nb_blockages,
        is_opl ? "OPL" : "META"
    );

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
        fprintf(file, "INSTANCE_NAME\t%s\n", file_name);
        fprintf(file, "NB_VERTICES\t\t%d\n", graph->nb_vertices);
        fprintf(file, "NB_ARCS\t\t%d\n", graph->nb_arcs);
        fprintf(file, "NB_REQUESTS\t\t%d\n", graph->nb_requests);
        fprintf(file, "NB_BLOCKAGES\t%d\n", graph->nb_blockages);
        fprintf(file, "\n");

        fprintf(file, "VERTICES\n");        
        for (i = 0; i < graph->nb_vertices; i++) {
            v = graph->vertices[i];
            fprintf(file, "%d\t%d\t%d\n",
                v->id + 1,
                v->x,
                v->y
            );
        }
        fprintf(file, "\n");

        fprintf(file, "ARCS\n"); 
        for (i = 0; i < graph->nb_arcs; i++) {
            a = graph->arcs[i];
            fprintf(file, "%d\t%d\t%d\n",
                a->source + 1,
                a->target + 1,
                a->cost
            );
        }
        fprintf(file, "\n");

        fprintf(file, "REQUESTS\n");
        for (i = 0; i < graph->nb_requests; i++) {
            r = graph->requests[i];
            fprintf(file, "%d\t%d\t%d\t%d\n",
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
            fprintf(file, "%d\t%d\n",
                b->source,
                b->target
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
