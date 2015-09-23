#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

char* get_file_name(int nbv, int nbe, int nbr, int nbb)
{
    char *file_name;
    sprintf(file_name, "v%d_a%d_r%d_b%d.dat", nbv, nbe, nbr, nbb);
    return file_name;
}

bool output_meta(
    int **vertices,
    int **arcs,
    int **requests,
    int **blockages,
    int nbv,
    int nbe,
    int nbr,
    int nbb)
{
    int i;
    char *file_name;
    FILE file;

    file_name = get_file_name(nbv, nbe, nbr, nbb);
    file = fopen(file_name, "w+");

    if (!file) {
        return false;
    } else {
        fprintf(file, "INSTANCE_NAME\t%s\n", file_name);
        fprintf(file, "NB_VERTICES\t%d\n", nbv);
        fprintf(file, "NB_EDGES\t%d\n", nbe);
        fprintf(file, "NB_REQUESTS\t%d\n", nbr);
        fprintf(file, "NB_BLOCKAGES\t%d\n", nbb);

        fprintf(file, "VERTICES\n");        
        for (i=0; i < nbv; i++) {
            fprintf(file, "%d\t%d\t%d\n", i + 1, vertices[i][0], vertices[i][1]);
        }

        fprintf(file, "\n");

        fprintf(file, "ARCS\n"); 
        for (i = 0; i < nbe; i++) {
            fprintf(file, "%d\t%d\t%d\n", arcs[i][0], arcs[i][1], arcs[i][2]);
        }

        fprintf(file, "\n");

        fprintf(file, "REQUESTS\n");
        for (i = 0; i < nbr; i++) {
            fprintf(file, "%d\t%d\t%d\n", requests[i][0], requests[i][1], requests[i][2], requests[i][3]);
        }

        fprintf(file, "\n");

        fprintf(file, "BLOCKAGES\n");
        for (i = 0; i < nbb; i++) {
            fprintf(file, "%d\t%d\t%d\n", blockages[i][0], blockages[i][1], blockages[i][2], blockages[i][3], blockages[i][4]);
        }

        fprintf(file, "\n");

        fprintf(file, "END");

        fclose(file);
        return true;
    }
}

bool output_opl(
    int **vertices,
    int **arcs,
    int **requests,
    int **blockages,
    int nbv,
    int nbe,
    int nbr,
    int nbb)
{
    return false;
}
