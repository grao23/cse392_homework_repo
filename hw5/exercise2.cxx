
#include <iostream>
#include <iomanip>
#include <random>
#include <sstream>
#include <vector>
#include <cmath>
using namespace std;
#include <mpi.h>

void transpose(MPI_Comm comm, double &value){
  int nprocs, procno; 
  MPI_Comm_size(comm, &nprocs); 
  MPI_Comm_rank(comm, &procno); 


  if (nprocs == 1)
    return; 
  
  //square matrix
  int N = (int)lround(sqrt((double)nprocs)); 
  int half = N/2; 
  int column = procno % N; 
  int row = procno / N; 
  

  //start by swapping (1,2) and (2,1)
  bool top_side = (row < half); 
  bool left_side = (column < half); 
  int row_partner = row; 
  int column_partner = column; 
  
  if (top_side && !left_side){
    row_partner = row + half; 
    column_partner = column - half; 
  } else if (!top_side && left_side){
    row_partner = row - half;
    column_partner = column + half;
  } 

  int partner = (row_partner * (N) + column_partner); 
  
  double recv = value; 
  MPI_Sendrecv(&value, 1, MPI_DOUBLE, partner, 0, &recv, 1, MPI_DOUBLE, partner, 0, comm, MPI_STATUS_IGNORE); 
  value = recv; 


  //splitting into 4 quadrants 
  
  int row_quadrants = (row >= half); 
  int column_quadrant = (column >= half); 
  int quad_id = 2 * row_quadrants + column_quadrant; 
  
  int local_row = row % half; 
  int local_column = column % half; 
  int key = local_row * half + local_column; 
  
  MPI_Comm subcomm; 
  MPI_Comm_split(comm, quad_id, key, &subcomm); 
  transpose(subcomm, value); 
  MPI_Comm_free(&subcomm); 
} 

int main() {
  MPI_Comm comm = MPI_COMM_WORLD; 
  int nprocs, procno; 
  MPI_Init(0,0); 
  MPI_Comm_size(comm, &nprocs); 
  MPI_Comm_rank(comm, &procno);
  int N = (int)lround(sqrt((double)nprocs)); 
  double value = (double)procno; 

  //printing original matrix
  vector<double> before_matrix; 
  if (procno == 0)
    before_matrix.resize(nprocs); 

  if (procno == 0)
    MPI_Gather(&value, 1, MPI_DOUBLE, before_matrix.data(), 1, MPI_DOUBLE, 0, comm); 
  else
    MPI_Gather(&value, 1, MPI_DOUBLE,NULL, 1, MPI_DOUBLE, 0, comm);

  if (procno == 0) { 
    cout << "Before the Matrix Transpose" << endl; 
    for (int i = 0; i <N; i++){
      for (int j = 0; j < N; j++)
	cout << setw(4) << before_matrix[i*N+j] << " "; 
      cout << endl;
    }
    cout << endl; 
  } 
  
  transpose(comm, value);

  //printing new matrix
  vector<double> after_matrix;
  if (procno == 0)
    after_matrix.resize(nprocs);

  if (procno == 0)
    MPI_Gather(&value, 1, MPI_DOUBLE, after_matrix.data(), 1, MPI_DOUBLE, 0, comm);
  else
    MPI_Gather(&value, 1, MPI_DOUBLE,NULL, 1, MPI_DOUBLE, 0, comm);

  if (procno == 0) {
    cout << "After the Matrix Transpose" << endl;
    for (int i = 0; i <N; i++){
      for (int j = 0; j < N; j++)
        cout << setw(4) << after_matrix[i*N+j] << " ";
      cout << endl;
    }
    cout << endl; 
  } 

  

 
  MPI_Finalize(); 
  return 0; 

} 

  
