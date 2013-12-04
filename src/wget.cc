#include <iostream>
#include <string>
#include "ThreadPool.h"
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <list>

#define MAX_BUFFER 4096
static const char* wget_cmd = "wget %s >/dev/null 2>1";

void wget(std::string url){ 
  char cmd[MAX_BUFFER];
  sprintf(cmd, wget_cmd, url.c_str());
  printf("Begin downloading %s...\n", url.c_str());
  if(system(cmd) == 0)
    printf("Downloaded %s.\n", url.c_str());
}

void wget_urls(std::list<std::string> urls, ThreadPool& pool){
  std::list<std::string>::iterator iter = urls.begin();
  for(; iter != urls.end(); ++iter)
    pool.enqueue(wget, *iter);
}

int main(int argc, char *argv[]){
  assert(argc > 1);
  ThreadPool pool(4, 100);
  
  if(argc == 2)
    wget(argv[1]);
  else{
    std::list<std::string> urls;
    for(int i = 1; i < argc; ++i)
      urls.push_back(argv[i]);
    
    wget_urls(urls, pool);
  } 

  return 0;
}
