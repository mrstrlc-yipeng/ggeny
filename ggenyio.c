#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Global variables for output formatting */
char *g_prefix_arcs = "ARCS";
char *g_prefix_request = "REQUEST";
char *g_prefix_blockages = "BLOCKAGES";
char *g_prefix_block = "";
char *g_suffix_block = "";
char *g_prefix_line = "";
char *g_separator_line = "    ";
char *g_suffix_line = "";
/******************************************/

char* get_file_name(int nbv, int nbe, int nbr, int nbb)
{
    char *file_name;
    sprintf(file_name, "v%d_a%d_r%d_b%d.dat", nbv, nbe, nbr, nbb);
    return file_name;
}

void format(char *file_type)
{
    if (0 == strcmp(file_type, "meta")) {
        g_prefix_arcs = "ARCS";
        g_prefix_request = "REQUEST";
        g_prefix_blockages = "BLOCKAGES";
        g_prefix_block = "";
        g_suffix_block = "";
        g_prefix_line = "";
        g_separator_line = "    ";
        g_suffix_line = "";
    } else {
        g_prefix_arcs = "arcs=";
        g_prefix_request = "requests=";
        g_prefix_blockages = "blockages=";
        g_prefix_block = "{";
        g_suffix_block = "}";
        g_prefix_line = "<";
        g_separator_line = ",";
        g_suffix_line = ">";
    }
}

bool output(char *file_type,
    int **vertices,
    int **arcs,
    int **requests,
    int **blockages,
    int nbv,
    int nbe,
    int nbr,
    int nbb)
{
    char *file_name;
    FILE file;

    file_name = get_file_name(nbv, nbe, nbr);
    file = fopen(file_name, "w+");

    if (!file) {
        return false;
    } else {
        fclose(file);
        return true;
    }
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
    return output(
        "meta",
        vertices,
        arcs,
        requests,
        blockages,
        nbv,
        nbe,
        nbr,
        nbb
    );
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
    return output(
        "opl",
        vertices,
        arcs,
        requests,
        blockages,
        nbv,
        nbe,
        nbr,
        nbb
    );
}
