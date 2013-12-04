#include <unistd.h>
#include <stdlib.h>

int get_nthreads(){
  int ncores = sysconf(_SC_NPROCESSORS_ONLN );
  char* str = getenv("N_THREADS");
  if(str == 0)
    return ncores * 2;

  return atoi(str);
}

