#include <cassert>
#include <iostream>
#include <memory>

#include "mpi.h"

using namespace std;

int main(int argc, char **argv) {
  MPI::Init(argc, argv);
  int rank = MPI::COMM_WORLD.Get_rank(), size = MPI::COMM_WORLD.Get_size();
  unique_ptr<int[]> data(new int[size]);
  MPI::COMM_WORLD.Gather(&rank, 1, MPI::INT, data.get(), 1, MPI::INT, 0);
  MPI::COMM_WORLD.Bcast(data.get(), size, MPI::INT, 0);
  for (int i = 0; i < size; ++i) assert(data[i] == i);
  MPI::Finalize();
  return 0;
}
