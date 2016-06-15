#include <cstdint>
#include <iostream>

#include "mpi.h"

using namespace std;

int main(int32_t argc, char **argv) {
  MPI::Init(argc, argv);
  int32_t rank = MPI::COMM_WORLD.Get_rank(), size = MPI::COMM_WORLD.Get_size(),
          sender, next = rank + 1, previous = rank - 1;
  if (next == size) next = 0;
  if (previous < 0) previous += size;
  if (size > 1) {
    MPI::COMM_WORLD.Isend(&rank, 1, MPI::INT, next, 0);
    MPI::COMM_WORLD.Recv(&sender, 1, MPI::INT, previous, 0);
    cout << rank << " received from " << sender << endl;
  }
  MPI::Finalize();
  return 0;
}
