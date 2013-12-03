#include <iostream>
#include <string>
#include "ThreadPool.h"
#include <getopt.h>
#include "ThreadPool.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <assert.h>
#include <string.h>

#define MAX_BUFFER 4096
static bool recursive = false;
static const char* compress_cmd = "gzip %s";

static struct option long_options[] = {
  {"--recursive",     no_argument,  0,  'r'},
  {0,                 0,            0,    0}
};

void usage(){
  printf("Usage : \n");
}

void init_args(int argc, char* argv[]){
  int opt_index = 0;
  int opt;
  
  while((opt = getopt_long(argc, argv, "r", long_options, &opt_index)) != -1){
    switch(opt){
      case 0:
        fprintf(stderr, "get_opt bug?\n");
        break;

      case '?':
        usage();
        break;
     
      case 'r':
        recursive = true;
        break;

      default:
        fprintf(stderr, "Invalid arguments (%c)\n", opt);
        usage();
    }
  }
}


void compress_singlefile(std::string filename){
  char cmd[MAX_BUFFER];
  sprintf(cmd, compress_cmd, filename.c_str());
  system(cmd);
}

void compress_directory(std::string dir, ThreadPool &pool){
  //recursively traverse the directory and compress files
  DIR* dfd = opendir(dir.c_str());
  struct dirent *dp;

  if(dfd == 0){
    fprintf(stderr, "Can't open dir %s\n", dir.c_str());
    exit(-1);
  }
  
  while((dp = readdir(dfd)) != NULL){
    if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
      continue;/* skip self and parent */
    
    std::string filename = dir + "/" + dp->d_name;
    
    struct stat stbuf;
    if(stat(filename.c_str(), &stbuf) == -1){
      fprintf(stderr, "Can't access file %s\n", filename.c_str());
      exit(-1);
    }

    if((stbuf.st_mode & S_IFMT) == S_IFDIR)
      compress_directory(filename, pool);
    else  
      pool.enqueue(compress_singlefile, filename);
  }

  closedir(dfd);
}

int main(int argc, char *argv[]){
 
  assert(argc > 1); 
  init_args(argc, argv);
  ThreadPool pool(4);
  
  std::string file = argv[argc - 1];
  
  if(file[file.size() - 1] == '/')
    file.resize(file.size() - 1);

  if(!recursive)
    compress_singlefile(file);
  else
    compress_directory(file, pool);
  
  return 0;
}
