#include <omp.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

//reads input
int read_input(int ifile) {
  std::ifstream in("input" + std::to_string(ifile));
  if (!in) {
    std::cerr << "Input did not open" << ifile << "\n";
    std::exit(1);
  }
  int value;
  in >> value;
  return value;
}


int transform(int *inputs, int ifile) {
  int output = 0;
  for (int iifile = 0; iifile <= ifile; iifile++) {
    output += inputs[iifile];
  }
  return output;
}

//write integer out
void writing_output(int ifile, int value) {
  std::ofstream out("output" + std::to_string(ifile));
  if (!out) {
    std::cerr << "Could not open output" << ifile << "\n";
    std::exit(1);
  }
  out << value << "\n";
}

int main() {
  const int nfiles = 3;

  std::vector<int> inputs(nfiles, 0);
  std::vector<int> outputs(nfiles, 0);\
  int *inptr = inputs.data();
  int *outptr = outputs.data();

    #pragma omp parallel
  {
        #pragma omp single
    {
      for (int ifile = 0; ifile < nfiles; ifile++) {
	// read inputs
#pragma omp task firstprivate(ifile) depend(out: inptr[ifile])
	{
	  inptr[ifile] = read_input(ifile);
	}
	// output transformation
#pragma omp task firstprivate(ifile) depend(in: inptr[0:ifile+1]) depend(out: outptr[ifile])
	{
	  outptr[ifile] = transform(inptr, ifile);
	}
	// write outputs
#pragma omp task firstprivate(ifile) depend(in: outptr[ifile])
	{
	  writing_output(ifile, outptr[ifile]);
	}
      }
            #pragma omp taskwait
    }
  }
  std::cout << "done\n";
  return 0;
}
