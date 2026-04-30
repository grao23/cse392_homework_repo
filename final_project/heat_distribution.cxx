#include <mpi.h>
#include <vector>
#include <iostream> 
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <algorithm>

//mdspan 2D view

struct mdspan2d{
  double* data; 
  int rows, cols; 
  
  double& operator()(int i, int j){
    return data[i * cols + j]; 
  } 

  const double& operator()(int i, int j) const{
    return data[i*cols+j]; 
  } 

}; 


int main(int argc, char** argv){
  MPI_Init(&argc, &argv); 
  MPI_Comm world = MPI_COMM_WORLD; 
  int rank, nprocs; 
  
  MPI_Comm_rank(world, &rank); 
  MPI_Comm_size(world, &nprocs); 
  
  //global grid size and the number of time steps
  int global_nx = 100;
  int global_ny = 100;
  int steps = 500;

  if (argc > 1) global_nx = std::atoi(argv[1]); 
  if (argc > 2) global_ny = std::atoi(argv[2]);
  if (argc > 3) steps = std::atoi(argv[3]);

  //cartesian communicator
  int dims[2] = {0,0}; 
  MPI_Dims_create(nprocs,2,dims); 
  
  //non-periodic boundary conditions
  int periods[2] = {0,0};
  MPI_Comm cart_comm; 
  
  MPI_Cart_create(world, 2, dims, periods, 1, &cart_comm);
  
  int cart_rank; 
  MPI_Comm_rank(cart_comm, &cart_rank); 
  
  int coords[2]; 
  MPI_Cart_coords(cart_comm, cart_rank, 2, coords); 
  
  int north, south, west, east; 
  MPI_Cart_shift(cart_comm, 0,1, &north, &south); 
  MPI_Cart_shift(cart_comm, 1,1, &west, &east);

  //global grid divides evenly
  int local_nx = global_nx / dims[0]; 
  int local_ny = global_ny / dims[1];
  
  if (global_nx % dims[0] != 0 || global_ny % dims[1] != 0){
    if (rank == 0) {
      std::cerr << "Error, grid not dividing evenly \n"; 
    } 
    MPI_Finalize(); 
    return 1; 
  }
  
  //adding cells to take values from neighbouring cells
  //to be able to calculate values locally

  std::vector<double> current_data((local_nx+2) * (local_ny + 2), 0.0); 
  std::vector<double> next_data((local_nx+2) * (local_ny + 2), 0.0);

  mdspan2d current{current_data.data(), local_nx+2, local_ny+2};
  mdspan2d next{next_data.data(), local_nx+2, local_ny+2};

  //initial condition
  int start_x = coords[0]*local_nx; 
  int start_y = coords[1]*local_ny; 
  
  for (int i=1; i<=local_nx; i++){
    for (int j=1; j<=local_ny; j++){
      int global_i = start_x + (i-1); 
      int global_j = start_y + (j-1); 

      //gives hot source at center of grid
      if (global_i >= global_nx / 2 - 10 && global_i < global_nx / 2 + 10 && global_j >= global_ny / 2 -10 && global_j < global_ny / 2 + 10){
	current(i,j) = 100.0; 
      } 
    } 
  } 
  
  //controls how fast the heat spreads
  double alpha = 0.1;
  
  MPI_Datatype column_type; 
  MPI_Type_vector(local_nx,1, local_ny + 2, MPI_DOUBLE, &column_type); 
  MPI_Type_commit(&column_type); 
  
  double start_time = MPI_Wtime(); 
  for (int step = 0; step < steps; step++){
    //exchanging north/south rows
    MPI_Sendrecv(&current(1,1), local_ny, MPI_DOUBLE, north, 0, 
		 &current(local_nx+1,1), local_ny, MPI_DOUBLE, south, 0, 
		 cart_comm, MPI_STATUS_IGNORE); 
    
    MPI_Sendrecv(&current(local_nx,1), local_ny, MPI_DOUBLE, south, 1,
                 &current(0,1), local_ny, MPI_DOUBLE, north, 1,
                 cart_comm, MPI_STATUS_IGNORE);

    //exchanging west/east columns

    MPI_Sendrecv(&current(1,1), 1, column_type, west, 2,
                 &current(1, local_ny + 1), 1, column_type, east, 2,
                 cart_comm, MPI_STATUS_IGNORE);
    MPI_Sendrecv(&current(1,local_ny), 1, column_type, east, 3,
                 &current(1, 0), 1, column_type, west, 3,
                 cart_comm, MPI_STATUS_IGNORE);

    //Heat diffusion stencil
    for (int i=1; i <= local_nx; i++){
      for (int j=1; j<= local_ny; j++){
	next(i,j) = current(i,j) + alpha * (current(i-1,j) + current(i+1,j) + current(i,j-1) + current(i,j+1) - 4.0*current(i,j)); 
      } 
    } 

    std::swap(current_data, next_data);
    current.data = current_data.data(); 
    next.data = next_data.data(); 
  } 

  double elapsed = MPI_Wtime() - start_time; 
  double local_sum = 0.0; 

  for (int i=1; i<= local_nx; i++){
    for (int j=1; j<= local_ny;j++){
      local_sum += current(i,j); 
    }
  } 

  double global_sum = 0.0; 
  double max_time = 0.0; 
  
  MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, cart_comm); 
  MPI_Reduce(&elapsed, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, cart_comm);

  if (cart_rank == 0){
    std::cout << "2d heat diffusion \n"; 
    std::cout << "Global grid: " << global_nx << "x" << global_ny << "\n"; 
    std::cout << "Process grid " << dims[0] << "x" << dims[1] << "\n";
    std::cout << "Steps: " << steps << "\n"; 
    std::cout << "Total heat: " << global_sum << "\n"; 
    std::cout << "Runtime: " << max_time << "seconds \n"; 
  } 

  MPI_Type_free(&column_type); 
  MPI_Comm_free(&cart_comm); 

  MPI_Finalize(); 
  return 0; 
}




