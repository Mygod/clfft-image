#include <cstdint>
#include <iostream>

#include "mpi.h"

#define N 4

using namespace std;

int32_t matrix[N][N];

int main(int argc, char **argv) {
  MPI::Init(argc, argv);
  MPI::Datatype diagonal = MPI::INT.Create_vector(N, 1, N + 1);
  diagonal.Commit();
  int rank = MPI::COMM_WORLD.Get_rank();
  if (!rank) {
    cout << "Generating matrix:" << endl;
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) cout << (matrix[i][j] = i * N + j) << ' ';
      cout << endl;
    }
  }
  MPI::COMM_WORLD.Bcast(&matrix[0][0], 1, diagonal, 0);
  cout << "Received by " << rank << ": ";
  for (int i = 0; i < N; ++i) cout << matrix[i][i] << ' ';
  cout << endl;
  diagonal.Free();
  MPI::Finalize();
  return 0;
}
