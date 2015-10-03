#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "ggenygraph.h"
#include "ggenyio.h"
#include "ggenyrand.h"

// flags for graph file output types
static int is_output_meta = 1;
static int is_output_opl = 1;

void main(int argc, char **argv)
{
    // arguments for graph generation
    int grid_size;
    int unit_cost;
    int nb_requests;
    int nb_blockages;

    Graph *graph;

    // arguments for options reading
    int opt;
    static struct option long_options[] = {
        {"without-meta", no_argument, &is_output_meta, 0},
        {"without-opl",  no_argument, &is_output_opl,  0},

        {"nb-blockages", required_argument, 0, 'b'},
        {"unit-cost",    required_argument, 0, 'c'},
        {"nb-requests",  required_argument, 0, 'r'},
        {"grid-size",    required_argument, 0, 's'},
        
        {0, 0, 0, 0}
    };

    while (1) {
        // getopt_long stores the option index here.
        int option_index = 0;

        opt = getopt_long (argc, argv, "b:c:r:s:", long_options, &option_index);

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

            case '?':
                // getopt_long already printed an error message.
                break;

            default:
                abort();
        }
    }

    graph = compute_grid_graph(
        grid_size,
        unit_cost,
        nb_requests,
        nb_blockages
    );

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
