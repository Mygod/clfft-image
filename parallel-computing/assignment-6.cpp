#include <algorithm>
#include <cassert>
#include <memory>
#include <iostream>

#include "mpi.h"

#define N 30000

using namespace std;

int main(int argc, char **argv) {
  MPI::Init(argc, argv);
  int rank = MPI::COMM_WORLD.Get_rank(), size = MPI::COMM_WORLD.Get_size();
  assert(size <= N);
  int from = rank * N / size, to = (rank + 1) * N / size;
  unique_ptr<double[]> vectorPart(new double[to - from]);
  for (int i = from; i < to; ++i) {
    double result = 0.;
    for (int j = i * N + 1, top = j + N; j < top; ++j) result += 1. / j;
    vectorPart[i - from] = result;
  }
  if (rank) MPI::COMM_WORLD.Gatherv(vectorPart.get(), to - from, MPI::DOUBLE,
                                    nullptr, nullptr, nullptr, MPI::DOUBLE, 0); else {
    unique_ptr<int[]> recvCounts(new int[N]), displs(new int[N + 1]);
    displs[0] = 0;
    for (int i = 0, j = 1; i < N; i = j++) recvCounts[i] = (displs[j] = j * N / size) - displs[i];
    unique_ptr<double[]> vector(new double[N]);
    MPI::COMM_WORLD.Gatherv(vectorPart.get(), to - from, MPI::DOUBLE, vector.get(),
                            recvCounts.get(), displs.get(), MPI::DOUBLE, 0);
    for (int i = 0; i < N - 1; ++i) cout << vector[i] << ' ';
    cout << vector[N - 1] << endl;
  }
  MPI::Finalize();
  return 0;
}
