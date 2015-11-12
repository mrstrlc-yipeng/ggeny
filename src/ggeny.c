#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "ggenygraph.h"
#include "ggenyio.h"
#include "ggenyrand.h"

// flags for display help message
static int is_help = 0;

// flags for graph file output types
static int is_output_meta = 1;
static int is_output_opl = 1;

// flags for graph file input types
static int is_input_meta = 0;
static int is_input_opl = 0;

void print_usage();
void output(Graph *graph);

void main(int argc, char **argv)
{
    // arguments for graph generation
    int grid_size = 3;
    int unit_cost = 1;
    int nb_requests = 0;
    int nb_blockages = 1;

    // arguments for graph load
    char *filename;

    // percentage of multiarcs, [0-100]
    int per_multiarcs = 0;

    Graph *graph;

    // arguments for options reading
    int opt;
    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},

        {"without-meta", no_argument, &is_output_meta, 0},
        {"without-opl",  no_argument, &is_output_opl,  0},

        {"input-meta", no_argument, &is_input_meta, 1},
        {"input-opl",  no_argument, &is_input_opl,  1},

        {"nb-blockages",  required_argument, 0, 'b'},
        {"unit-cost",     required_argument, 0, 'c'},
        {"nb-requests",   required_argument, 0, 'r'},
        {"grid-size",     required_argument, 0, 's'},
        {"per-multiarcs", required_argument, 0, 'p'},

        {"filename", required_argument, 0, 'f'},
        
        {0, 0, 0, 0}
    };

    while (1) {
        // getopt_long stores the option index here.
        int option_index = 0;

        opt = getopt_long (argc, argv, "h:b:c:r:s:p:f:", long_options, &option_index);

        // Detect the end of the options.
        if (opt == -1) break;

        switch (opt) {
            case 0:
                // If this option set a flag, do nothing else now.
                if (long_options[option_index].flag != 0) break;
                printf ("option %s set", long_options[option_index].name);
                if (optarg) printf (" with arg %s", optarg);
                printf ("\n");
                break;

            case '?':
            case 'h':
                is_help = 1;
                print_usage();
                break;

            case 'b':
                nb_blockages = atoi(optarg);
                break;

            case 'c':
                unit_cost = atoi(optarg);
                break;

            case 's':
                grid_size = atoi(optarg);
                break;

            case 'r':
                nb_requests = atoi(optarg);
                break;

            case 'p':
                per_multiarcs = atoi(optarg);
                break;

            case 'f':
                filename = optarg;
                break;

            default:
                abort();
        }
    }

    if (!is_help) {
        if (is_input_meta) {
            graph = input_meta(filename);
        } else if (is_input_opl) {
            graph = input_opl(filename);
        } else {
            graph = compute_grid_graph(
                grid_size,
                unit_cost,
                nb_requests,
                nb_blockages
            );

            if (graph != NULL) output(graph);
        }

        if (graph == NULL) return;

        if (per_multiarcs > 0) {
            patch_multiarcs_2(graph, (float) per_multiarcs / 100);
            output(graph);
        }

        free(graph);
    }
}

void print_usage()
{
    printf("usage: ggeny [options]\n\n");
    printf("  options:\n");
    printf("    -b, --nb-blockages\t\tinteger number of blockages (default to 1).\n");
    printf("    -c, --unit-cost\t\treal number of the cost set to every arcs (default to 1).\n");
    printf("    -h, --help\t\t\tdisplay usages.\n");
    printf("    -r, --nb-requests\t\tinteger number of flow requests (default to 0).\n");
    printf("    -s, --grid-size\t\tinteger number of the grid size (default to 3).\n");
    printf("    -p, --per-multiarcs\t\tinteger number of percentage of the multiarcs (default to 0).\n");
    printf("    -f, --filename\t\tstring filename of the graph file to input.\n");
    printf("\n");
    printf("    --without-meta\t\tprevent output file from metadata format.\n");
    printf("    --without-opl\t\tprevent output file from opl format.\n");
    printf("\n");
    printf("    --input-meta\t\tspecify an input graph file of metadata format.\n");
    printf("    --input-opl\t\tspecify an input graph file of opl format.\n");
    printf("\n");
    printf("  examples:\n");
    printf("    1. ggeny --without-opl:\n");
    printf("       generate a default grid graph exported only on meta file with");
    printf(" 3x3 vertices, arc costs of 1, 0 requests and 1 blockage.\n");
    printf("    2. ggeny --without-meta -s 5 -b 4:\n");
    printf("       generate a grid graph exported only on opl file with");
    printf(" 5x5 vertices, arc costs of 1, 0 requests and 4 blockages.\n");
}

void output(Graph *graph) {
    if (is_output_meta) {
        if (output_meta(graph)) {
            printf("META output file ok.\n");
        } else {
            printf("META output file failed.\n");
        }
    }

    if (is_output_opl) {
        if (output_opl(graph)) {
            printf("OPL output file ok.\n");
        } else {
            printf("OPL output file failed.\n");
        }
    }
}
