#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <sys/timeb.h>

//----------------------------------------------------------------------------------
//global variables

//problems data
int      nb_requests;   //number of requests
int      nb_edges;      //number of edges
int      grid_size;     //grid size (4)x4, (5)x5, ...
int      seed;
int      **coord;       //stock coordinates x,y and the cost
char     *name;                      //input file name
FILE     *outFileOPL, *outFileMETA;  //output file name

int      getRand_ij ( int *seed, int i, int j );
float    randp(int *ix);
void     compute_grid(int coordX, int coordY);          

//----------------------------------------------------------------------------------
//grid size
//parameters: seed, grid_size

void main ( int argc, char ** argv ){  
    char  nameOPL[256], nameMETA[256];
    char  aux[20]="";
    int   i, j, k, cost, nb_nodes;
    int   coordX, coordY;

    if ( argc < 2 ) printf("Missing parameters\n");
    else{
        grid_size = atoi(argv[1]);
    }
    seed = atoi(argv[2]); 
    
    nb_nodes = (grid_size*grid_size);

    //Only the point bottom-left is randomly generated, the other are computed using the edge cost
    coordX = getRand_ij(&seed, 0, 100);
    coordY = getRand_ij(&seed, 0, 100);
    nb_edges = 2 * grid_size * ( grid_size-1 );
    compute_grid( coordX, coordY ); 
    
    strcpy(nameMETA,"g");
    strcat(nameMETA, _itoa(grid_size, aux, 10));
    strcat(nameMETA, "x");
    strcat(nameMETA, _itoa(grid_size, aux, 10));
    strcat(nameMETA, ".txt");   
        
    if((outFileMETA = fopen(nameMETA,"a+"))!=NULL){
        fprintf(outFileMETA, "INSTANCE_NAME\t %s\n", nameMETA);
        fprintf(outFileMETA, "NB_VERTICES\t %d\n", grid_size*grid_size);
        fprintf(outFileMETA, "NB_EDGES\t %d\n", nb_edges);    

        fprintf(outFileMETA, "VERTICES \n");        
        for ( i=0; i < (grid_size * grid_size); i++ ){
            fprintf( outFileMETA, "%d\t%d\t%d\n", i, coord[i][0], coord[i][1] );        
        }

        fprintf(outFileMETA, "\nEDGES \n"); 
        //writing lignes
        k=0;
        for( j = 0; j < grid_size ; j++){       
            for ( i=0; i < grid_size-1 ; i++ ){
                fprintf( outFileMETA, "%d\t%d\t%d\n", i+k, i+k+1, coord[i+k+1][0]-coord[i+k][0] );      
            }
            k += grid_size;
        }

        //writing colums
        k=0;
        for( j = 0; j < grid_size-1 ; j++){
            for ( i=0; i < grid_size ; i++ ){
                fprintf( outFileMETA, "%d\t%d\t%d\n", k+i, k+grid_size+i, coord[k+grid_size+i][1]-coord[k+i][1] );              
            }
            k +=grid_size;
        }
    }
    else{
        printf("output file fail\n");
    }

    strcpy(nameOPL,"g");
    strcat(nameOPL, _itoa(grid_size, aux, 10));
    strcat(nameOPL, "x");
    strcat(nameOPL, _itoa(grid_size, aux, 10));
    strcat(nameOPL, ".dat");    

    if((outFileOPL = fopen(nameOPL,"a+"))!=NULL){
        fprintf(outFileOPL, "V =%d;\n", grid_size*grid_size);
        fprintf(outFileOPL, "A = %d;\n", 2*nb_edges);       
        
        fprintf(outFileOPL, "\narcs = {\n");    
        //writing lignes
        k=0;
        for( j = 0; j < grid_size ; j++){       
            for ( i=0; i < grid_size-1 ; i++ ){
                cost = coord[i+k+1][0]-coord[i+k][0] ;
                fprintf( outFileOPL, "<%d, %d, %d>,\n", i+k, i+k+1, cost );     
                fprintf( outFileOPL, "<%d, %d, %d>,\n", i+k+1, i+k, cost );     
            }
            k += grid_size;
        }
        
        //writing colums
        k=0;
        for( j = 0; j < grid_size-1 ; j++){
            for ( i=0; i < grid_size ; i++ ){
                cost = coord[k+grid_size+i][1]-coord[k+i][1];
                fprintf( outFileOPL, "<%d, %d, %d>,\n", k+i, k+grid_size+i, cost ); 
                fprintf( outFileOPL, "<%d, %d, %d>,\n", k+grid_size+i,  k+i, cost );    
            }
            k +=grid_size;
        }
        fprintf(outFileOPL, "};\n\n");
    }
    else{
        printf("output file fail\n");
    }

    fclose(outFileOPL);  
    fclose(outFileMETA);
}

//----------------------------------------------------------------------------------

 int getRand_ij( int *seed, int i, int j ){
    randp(seed);
    return  (int)((double)*seed/((double)2147483647/((double)(j-i+1))))+i;
 }//end_get_rand_ij

//------------------------------------------------------------------------------

 float randp(int *ix){
    int xhi, xalo, leftlo, fhi, k;
    const int A     = 16807;
    const int P     = 2147483647;
    const int b15   = 32768;
    const int b16   = 65536;
    xhi = *ix/b16;
    xalo    = (*ix-xhi*b16)*A;
    leftlo  = xalo/b16;
    fhi = xhi*A+leftlo;
    k   = fhi/b15;
    *ix = (((xalo-leftlo*b16)-P)+(fhi-k*b15)*b16)+k;
    if (*ix < 0) *ix = *ix + P;
        return (float)(*ix*4.656612875e-10);
  }//end_randp

//----------------------------------------------------------------------------------
//aloca e inicializa estruturas de dados
void compute_grid( int coordX, int coordY ){
    int    i, j, id, l, c;
    int    *cost_ligne, *cost_column;
    
    cost_ligne   = (int *) malloc(sizeof(int) * grid_size);           
    cost_column = (int *) malloc(sizeof(int) * grid_size);            

    coord = (int **) malloc(sizeof(int*) * (grid_size*grid_size) );            
    for( j=0; j < (grid_size*grid_size) ; j++ ){                  
       coord[j] = (int *) malloc(sizeof(int) * 2);            
    }//end_for_j

    //compute costs
    for ( i =0; i < grid_size -1; i++ ){
        cost_ligne[i] = getRand_ij(&seed, 0, 100);
        cost_column[i] = getRand_ij(&seed, 0, 100);
    }//end_for

    id=0; c=0;
    for(i=0; i < grid_size;i++){    
        l=0; 
        for(j=0; j < grid_size;j++){

            if(i==0) coord[id][1] = coordY; 
            else { 
                coord[id][1] = coord[(id-grid_size)][1] + cost_column[c];                
            }

            if(j==0) coord[id][0] = coordX; 
            else { 
                coord[id][0] = coord[id-1][0] + cost_ligne[l];
                l++;
            }
            id++;
        }//end_for  
        if(i==0) c=0;
        else  c++;
    }//end_for

        
    
        

    free(cost_ligne);
    free(cost_column);

}//end

//----------------------------------------------------------------------------------
//desaloca estruturas de dados

void desaloca(){
  int i;
  
  for(i=0;i<(grid_size*grid_size);i++){    
     free(coord[i]);     
  }free(coord);       
        
}//fim_desaloca

//----------------------------------------------------------------------------------

