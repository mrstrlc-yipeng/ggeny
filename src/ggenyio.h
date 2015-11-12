#include <stdio.h>
#include <stdbool.h>

#include "ggenygraph.h"

#ifndef GGENY_IO_H
#define GGENY_IO_H

extern const char *DIR;
extern const int FILE_NAME_META;
extern const int FILE_NAME_OPL;

// function declarations
// major feature functions
bool output_meta(Graph *graph);
bool output_opl(Graph *graph);
Graph* input_meta(char *file_name);
Graph* input_opl(char *file_name);

// utilitiy functions
char* get_file_name(Graph *graph, int is_opl);
FILE* get_file(char *file_name);

#endif
