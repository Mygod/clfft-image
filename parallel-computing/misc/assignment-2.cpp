#include <cstdint>
#include <iostream>

#include "mpi.h"

using namespace std;

// ceil(log(n)/log(2))
int main(int32_t argc, char **argv) {
  MPI::Init(argc, argv);
  int32_t rank = MPI::COMM_WORLD.Get_rank(), size = MPI::COMM_WORLD.Get_size(), data, i, target;
  if (rank) {
    i = rank | rank >> 1;
    i |= i >> 2;
    i |= i >> 4;
    i |= i >> 8;
    i = (i | i >> 16) + 1;
    target = rank & ~(i >> 1);
    MPI::COMM_WORLD.Recv(&data, 1, MPI::INT, target, 0);
    cout << rank << ": Data " << data << " received from " << target << endl;
  } else {
    data = argc;
    cout << rank << ": Data " << data << " generated" << endl;
    i = 1;
  }
  while ((target = rank | i) < size) {
    MPI::COMM_WORLD.Send(&data, 1, MPI::INT, target, 0);
    i <<= 1;
  }
  MPI::Finalize();
  return 0;
}
