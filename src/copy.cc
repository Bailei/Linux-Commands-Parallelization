#include <iostream>
#include <string>
#include "ThreadPool.h"
#include <unistd.h>

void fun(int i ){
  std::cout<<i<<std::endl;
  sleep(2);
}

int main(int argc, char *argv[]){
  ThreadPool pool(4, 1);

  for(int i = 0; i < 10; ++i)
    pool.enqueue(fun, i);
  std::cout<<"end"<<std::endl;

  return 0;
}
