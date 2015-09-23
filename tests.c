#include <stdio.h>
#include <stdint.h>

#include "ggenygraph.h"

//#include "xorshift.c"
//#include "ggenyio.c"

void test_xorshift(void);
void test_ggenygraph(void);
void test_ggenyio(void);

int main(void) {
    test_ggenygraph();
    return 0;
}

void test_xorshift(void) {

}

void test_ggenygraph(void) {
    int i;
    Graph *g = init_grid_graph(3, 0, 0);
    compute_grid_vertices(g, 0, 0, 1);
    for (i = 0; i < 9; i++) {
        printf("v[%d] = (%d,%d)\n", i + 1, g->vertices[i]->x, g->vertices[i]->y);
    }
}

void test_ggenyio(void) {
    
}
