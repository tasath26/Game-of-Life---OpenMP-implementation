#include "life.h"

size_t rows;
size_t columns;
size_t generations;
char** grid;
char** new_grid;

/* Direction Vectors: Down | Right | Up | Left | DownRight | Upright | UpLeft | DownLeft */
int row_dir[8] = {1,0,-1,0,1,-1,-1,1};
int col_dir[8] = {0,1,0,-1,1,1,-1,-1};

enum status{
  DEAD, 
  ALIVE
};


int init_grids(FILE *fp){
  
  grid = (char**)malloc(rows*sizeof(char*));
  new_grid = (char**)malloc(rows*sizeof(char*));

  for(size_t i = 0; i < rows; i++){
    grid[i] = (char*)malloc(columns*sizeof(char));
    new_grid[i] = (char*)malloc(columns*sizeof(char));
  }

  char c;
  int row = 0; 
  int col = 0;
  for(size_t i = 0; i < rows; i++){
    for(size_t j = 0; j < columns; j++){
      c = getc(fp);
      if(c == ' ' || c == '*'){
        grid[i][j] = c;
      }else{
        j--;
      }
    }
    fgetc(fp);
  }

  return 0;
}


void game_of_life(){

  int print_message = 1;
  int status,nr,nc;
  //int new_status;
  int alive_neighbors = 0;
  int dead_neighbors = 0;
  char** tmp;

  for(int gens = 0; gens < generations; gens++){
  #pragma omp parallel private(nr,nc,status,alive_neighbors,dead_neighbors)
  {

  
  if(print_message == 1){
    #pragma omp single
    {
    printf("Running using %d threads...\n", omp_get_num_threads());
    print_message = 0;
    }
  }

    #pragma omp for
    for(int i = 0; i < rows; i++){
      for(int j = 0; j < columns; j++){

          if(grid[i][j] == '*'){
            status = ALIVE;
          }else{
            status = DEAD;
          }

          alive_neighbors = 0;
          dead_neighbors = 0;

          for(int k = 0; k < 8; k++){
            nr = i + row_dir[k];
            nc = j + col_dir[k];

            if(nr < 0 || nr >= rows || nc < 0 || nc >= columns){ continue; }

            if(grid[nr][nc] == '*'){
              alive_neighbors++;
            }else{
              dead_neighbors++;
            }
  
          }

          if((status == ALIVE && alive_neighbors < 2) || (status == ALIVE && alive_neighbors > 3)){
            new_grid[i][j] = ' ';
            //new_status = DEAD;
          }else if((status == ALIVE && (alive_neighbors == 2 || alive_neighbors == 3)) || (status == DEAD && alive_neighbors == 3)){
            new_grid[i][j] = '*';
            //new_status = ALIVE; 
          }else{
            new_grid[i][j] = grid[i][j];
            //new_status = status;
          }
      
      }
    }

    } /* End of parallel part */

    tmp = grid;
    grid = new_grid;
    new_grid = tmp;
 
  }
}


int main(int argc, char** argv){

  if(argc != 4){
    printf("Argument error\n  Usage ./life <input file> <generations> <output file>\n");
  }

  double exec_time;
  char input[1024];
  char *token;
  FILE *fp = fopen(argv[1],"r");
  if(!fp){
    printf("Error with opening file\n");
    exit(1);
  }
  generations = atoi(argv[2]);

  fscanf(fp, "%lu %lu", &rows, &columns);
  fgetc(fp);

  if(init_grids(fp) == -1){
    printf("Error in initializing grid\n");
    exit(1);
  }

  clock_gettime(CLOCK_MONOTONIC, &start_time); 
  game_of_life();
  clock_gettime(CLOCK_MONOTONIC, &finish_time);

  fclose(fp);

  FILE *ofp = fopen(argv[3], "w");

  for(size_t i = 0; i < rows; i++){
    for(size_t j = 0; j < columns; j++){
      fprintf(ofp,"|%c", grid[i][j]);
    }
    fprintf(ofp,"|\n");
  }


  exec_time = (finish_time.tv_sec - start_time.tv_sec) + (finish_time.tv_nsec - start_time.tv_nsec) / 1.0e9;
  printf("total execution time = %f sec.\n", exec_time);

  fclose(ofp);
  return 0;


}
