#ifndef GGENY_RAND_H
#define GGENY_RAND_H

float _randp(int *ix){
    int xhi, xalo, leftlo, fhi, k;
    
    const int A     = 16807;
    const int P     = 2147483647;
    const int b15   = 32768;
    const int b16   = 65536;
    
    xhi     = *ix/b16;
    xalo    = (*ix-xhi*b16)*A;
    leftlo  = xalo/b16;
    fhi     = xhi*A+leftlo;
    k       = fhi/b15;
    *ix     = (((xalo-leftlo*b16)-P)+(fhi-k*b15)*b16)+k;
    
    if (*ix < 0) *ix = *ix + P;

    return (float)(*ix*4.656612875e-10);
}

// rand an integer in the interval [i,j]
int rand_ij(int *seed, int i, int j){
    _randp(seed);
    return  (int)((double)*seed/((double)2147483647/((double)(j-i+1))))+i;
}//end_get_rand_ij

#endif
