#include <algorithm>
#include <cassert>
#include <memory>
#include <iostream>
#include <cstring>

#include "mpi.h"

#define N 25
#define EPSILON 1e-6
#define TAG_TO_PREVIOUS 0
#define TAG_TO_NEXT 1
#define TAG_OUTPUT 2

using namespace std;

typedef double layer[N][N];

inline double sqr(double value) {
  return value * value;
}

inline double getMatrixImpl(int fromZ, int toZ, double (*const&previous)[N], double (*const&next)[N],
                            layer *const&matrix, int z, int y, int x) {
  if (z < fromZ) return previous[y][x];
  if (z >= toZ) return next[y][x];
  return matrix[z - fromZ][y][x];
}

#define getMatrix(z, y, x) getMatrixImpl(fromZ, toZ, previous.get(), next.get(), matrix.get(), z, y, x)

int main(int argc, char **argv) {
  MPI::Init(argc, argv);
  auto typeLayer = MPI::DOUBLE.Create_contiguous(N * N);
  typeLayer.Commit();
  int rank = MPI::COMM_WORLD.Get_rank(), size = MPI::COMM_WORLD.Get_size();
  assert(size <= N);
  int fromZ = rank * N / size, toZ = (rank + 1) * N / size;
  unique_ptr<layer[]> matrix(new layer[toZ - fromZ]), temp(new layer[toZ - fromZ]);
  for (int z = fromZ; z < toZ; ++z) fill(*matrix[z - fromZ], *matrix[z - fromZ + 1], z ? z >= N - 1 ? 1.0 : 0.0 : -1.0);
  int startZ = fromZ ? fromZ : 1, endZ = toZ >= N ? N - 1 : toZ, iterations = 0;
  memcpy(temp.get(), matrix.get(), sizeof(double) * N * N * (toZ - fromZ));
  unique_ptr<double[][N]> previous(new layer), next(new layer);
 nextIteration:
  {
    double diffSquaredLocal = 0.0;
    ++iterations;
    if (rank) MPI::COMM_WORLD.Isend(matrix[0], 1, typeLayer, rank - 1, TAG_TO_PREVIOUS);
    if (rank < size - 1) {
      MPI::COMM_WORLD.Isend(matrix[toZ - fromZ - 1], 1, typeLayer, rank + 1, TAG_TO_NEXT);
      MPI::COMM_WORLD.Recv(next.get(), 1, typeLayer, rank + 1, TAG_TO_PREVIOUS);
    }
    if (rank) MPI::COMM_WORLD.Recv(previous.get(), 1, typeLayer, rank - 1, TAG_TO_NEXT);
    for (int z = startZ; z < endZ; ++z) for (int y = 1; y < N - 1; ++y) for (int x = 1; x < N - 1; ++x)
      diffSquaredLocal += sqr((temp[z - fromZ][y][x] = (getMatrix(z, y, x - 1) + getMatrix(z, y, x + 1) +
          getMatrix(z, y - 1, x) + getMatrix(z, y + 1, x) + getMatrix(z - 1, y, x) + getMatrix(z + 1, y, x)) / 6.0) -
        matrix[z - fromZ][y][x]);
    // syncing point here so no need to wait for Isend to finish
    double diffSquared;
    MPI::COMM_WORLD.Allreduce(&diffSquaredLocal, &diffSquared, 1, MPI::DOUBLE, MPI::SUM);
    if (diffSquared >= EPSILON * EPSILON) {
      swap(matrix, temp);
      goto nextIteration;
    }
  }
  if (rank) MPI::COMM_WORLD.Recv(&rank, 0, MPI::INT, rank - 1, TAG_OUTPUT); // only a signal
  else cerr << "Jacobi iteration finished with " << iterations << " iteration(s)." << endl;
  for (int z = fromZ; z < toZ; ++z) {
    cout << "Layer " << z << ':' << endl;
    for (int y = 0; y < N; ++y) {
      for (int x = 0; x < N - 1; ++x) cout << matrix[z - fromZ][y][x] << ' ';
      cout << matrix[z - fromZ][y][N - 1] << endl;
    }
    cout << endl;
  }
  if (rank < size - 1) MPI::COMM_WORLD.Send(&rank, 0, MPI::INT, rank + 1, TAG_OUTPUT);
  MPI::Finalize();
  return 0;
}
