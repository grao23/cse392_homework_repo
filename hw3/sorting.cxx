

#include <iostream>
#include <iomanip>
#include <random>
#include <sstream>
#include <vector>
#include <cmath>
using namespace std;
#include <mpi.h>
#include <ctime> 
#include <cstdlib>

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  int rank = 0; 
  int P = 0; 
  
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
  MPI_Comm_size(MPI_COMM_WORLD, &P);

  srand(time(NULL) + rank); 
  int x = rand() % 100 + 1;  
  
  
  // initial state

  vector <int> global; 
  if (rank == 0) 
    global.resize(P);

  if (rank == 0) {
    MPI_Gather(&x,1, MPI_INT, global.data(),1, MPI_INT, 0, MPI_COMM_WORLD); 
  } else {
    MPI_Gather(&x,1, MPI_INT, 0 ,1, MPI_INT, 0, MPI_COMM_WORLD);
  } 

  
  if (rank == 0) {
    cout << "Initial array: \n"; 
    for (int i =0; i < P; i++)
      cout << "Rank " << i << ": " << global[i] << endl; 
    cout << endl; 
  } 

  // P/2 times

  int repeat = P/2; 
  
  for (int t =0; t < repeat; t++){
    int partner; 
    
    if (rank % 2 == 0)
      partner = rank + 1;
    else 
      partner = rank - 1; 

    if (partner < 0 || partner >= P)
      partner = MPI_PROC_NULL; 

    
    int y = 0; 
    
    MPI_Sendrecv(&x, 1, MPI_INT, partner, 0, &y, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
    
    if (partner != MPI_PROC_NULL){
      if (rank < partner)
	x = min(x,y); 
      else
	x = max(x,y); 
    }

    
    // for odd

    if (rank % 2 == 1)
      partner = rank + 1;
    else
      partner = rank - 1;

    if (partner < 0 || partner >= P)
      partner = MPI_PROC_NULL;


    MPI_Sendrecv(&x, 1, MPI_INT, partner, 0, &y, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    if (partner != MPI_PROC_NULL){
      if (rank < partner)
        x = min(x,y);
      else
        x = max(x,y);
    }

  } 


  // gather it all here

  if (rank == 0) {
    MPI_Gather(&x,1, MPI_INT, global.data(),1, MPI_INT, 0, MPI_COMM_WORLD);
  } else {
    MPI_Gather(&x,1, MPI_INT, 0 ,1, MPI_INT, 0, MPI_COMM_WORLD);

  } 

  if (rank == 0) {
    cout << "sorted array: \n"; 
    for (int i = 0; i < P; i++)
      cout << "rank " << i << ":  " << global[i] << endl; 
    cout << endl; 
  } 

  MPI_Finalize(); 
  return 0; 
} 



