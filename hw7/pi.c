/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming for Science and Engineering'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-6
 ****
 **** OpenMP Exercise
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

// 10 million
#define N 10000000

int main() {

  // step size
  double h = 1./(N+1);

  // start timer
  double t;
  double quarterpi;
  // Exercise:
  // 1 : put a `parallel' directive around the loop; Compile and run.
  //     -- Is the result correct? 
  //     -- How does the time change with the number of threads?
  //        for t in 1 2 4 8 12 ; do OMP_NUM_THREADS=$t ./pi ; done
  // 2 : also use a `for' directive. same questions.
  // 3 : put a `critical' directive in front of the update. same questions.
  // 4 : remove the critical and use a clause `reduction(+:quarterpi)'
/**** your code here ****/

//part 1

  t = omp_get_wtime(); 
  quarterpi = 0.;
#pragma omp parallel
  {
    for (int i=0; i<N; i++){
      double x = i*h;
      quarterpi += h * sqrt(1-x*x);
    } 
  }

  // stop timer and report
  t = omp_get_wtime()-t;
#pragma omp parallel
  if (omp_get_thread_num()==0)
    printf("Step 1: Pi=%12.8f in time on %2d threads: %7.5f\n",
	   4*quarterpi,omp_get_num_threads(),t);


  //part 2

  t = omp_get_wtime();
  quarterpi = 0.;
  
#pragma omp parallel for 
  for (int i=0; i<N; i++){
    double x = i*h;
    quarterpi += h * sqrt(1-x*x);
  }


  // stop timer and report
  t = omp_get_wtime()-t;
#pragma omp parallel
  if (omp_get_thread_num()==0)
    printf("Step 2: Pi=%12.8f in time on %2d threads: %7.5f\n",
           4*quarterpi,omp_get_num_threads(),t);


  //part 3

  t = omp_get_wtime();
  quarterpi = 0.;

#pragma omp parallel for
    for (int i=0; i<N; i++){
      double x = i*h;
#pragma omp critical
      quarterpi += h * sqrt(1-x*x);
    }
 

  // stop timer and report
  t = omp_get_wtime()-t;
#pragma omp parallel
  if (omp_get_thread_num()==0)
    printf("Step 3: Pi=%12.8f in time on %2d threads: %7.5f\n",
           4*quarterpi,omp_get_num_threads(),t);


  //part 4

  t = omp_get_wtime();
  quarterpi = 0.;

#pragma omp parallel for reduction(+:quarterpi)
  for (int i=0; i<N; i++){
    double x = i*h;
    quarterpi += h * sqrt(1-x*x);
  }


  // stop timer and report
  t = omp_get_wtime()-t;
#pragma omp parallel
  if (omp_get_thread_num()==0)
    printf("Step 4: Pi=%12.8f in time on %2d threads: %7.5f\n",
           4*quarterpi,omp_get_num_threads(),t);

  
  return 0;
}
