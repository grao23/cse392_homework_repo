/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-7
 ****
 **** MPI Exercise
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main(int argc,char **argv) {
  
  MPI_Init(&argc, &argv);

  MPI_Comm comm = MPI_COMM_WORLD; 
  int nprocs, myproc; 
  MPI_Comm_size(comm, &nprocs); 
  MPI_Comm_rank(comm, &myproc);

  int my_ints[10]; 
  int i; 
  
  printf("Process: %d  ", myproc);

  for (i = 0; i < 10; i++) {
    my_ints[i] = 10 * myproc + i;
    printf("%d ", my_ints[i]);
  }

  printf("\n");

  MPI_Finalize();
 
  return 0;

}
