#include <cstdint>
#include <iostream>

#include "mpi.h"

using namespace std;

int32_t main(int32_t argc, char **argv) {
  MPI::Init(argc, argv);
  int32_t rank = MPI::COMM_WORLD.Get_rank(), size = MPI::COMM_WORLD.Get_size(),
          sender, next = rank + 1, previous = rank - 1, bufferSize = 4 + MPI::BSEND_OVERHEAD;
  if (next == size) next = 0;
  if (previous < 0) previous += size;
  auto bufferArray = new uint8_t[bufferSize];
  auto buffer = reinterpret_cast<void*>(bufferArray);
  MPI::Attach_buffer(buffer, bufferSize);
  if (size > 1) {
    MPI::COMM_WORLD.Bsend(&rank, 1, MPI::INT, next, 0);
    MPI::COMM_WORLD.Recv(&sender, 1, MPI::INT, previous, 0);
    cout << rank << " received from " << sender << endl;
  }
  MPI::Detach_buffer(buffer);
  delete bufferArray;
  MPI::Finalize();
  return 0;
}
