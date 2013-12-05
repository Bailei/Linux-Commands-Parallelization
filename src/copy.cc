#include <iostream>
#include <string>
#include "ThreadPool.h"
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include "util.h"

static bool recursive = false;
static std::string src;
static std::string dst;

int cp(std::string dst, std::string src)
{ 
    const char* to = dst.c_str();
    const char* from = src.c_str();

    int fd_to, fd_from;
    char buf[4096];
    ssize_t nread;
    int saved_errno;

    fd_from = open(from, O_RDONLY);
    if (fd_from < 0)
        return -1;

    fd_to = open(to, O_WRONLY | O_CREAT | O_EXCL, 0666);
    if (fd_to < 0)
        goto out_error;

    while (nread = read(fd_from, buf, sizeof buf), nread > 0)
    {
        char *out_ptr = buf;
        ssize_t nwritten;

        do {
            nwritten = write(fd_to, out_ptr, nread);

            if (nwritten >= 0)
            {
                nread -= nwritten;
                out_ptr += nwritten;
            }
            else if (errno != EINTR)
            {
                goto out_error;
            }
        } while (nread > 0);
    }

    if (nread == 0)
    {
        if (close(fd_to) < 0)
        {
            fd_to = -1;
            goto out_error;
        }
        close(fd_from);

        /* Success! */
        return 0;
    }

  out_error:
    saved_errno = errno;

    close(fd_from);
    if (fd_to >= 0)
        close(fd_to);

    errno = saved_errno;
    return -1;
}

static struct option long_options[] = {
  {"--recursive",     no_argument,  0,  'r'},
  {0,                 0,            0,    0}
};

void usage(){
  printf("Usage : ./copy -r src_dir dst_dir\n");
  exit(-1);
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

void copy_directory(std::string rel_path, ThreadPool &pool){
  //recursively traverse the directory and compress files
  DIR* dfd = opendir((src + rel_path).c_str());
  struct dirent *dp;

  if(dfd == 0){
    fprintf(stderr, "Can't open dir %s\n", rel_path.c_str());
    exit(-1);
  }

  while((dp = readdir(dfd)) != NULL){
    if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
      continue;/* skip self and parent */
    
    std::string filename = rel_path + "/" + dp->d_name;
    
    struct stat stbuf;
    if(stat((src + filename).c_str(), &stbuf) == -1){
      fprintf(stderr, "Can't access file %s\n", filename.c_str());
      exit(-1);
    }

    if((stbuf.st_mode & S_IFMT) == S_IFDIR){
      mkdir((dst + filename).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
      copy_directory(filename, pool);
    }
    else{
      pool.enqueue(cp, dst + filename, src + filename);
    }
  }

  closedir(dfd);
}

int main(int argc, char *argv[]){
  init_args(argc, argv);
  
  int n_threads = get_nthreads();
  ThreadPool pool(n_threads, 100);
  
  src = argv[argc - 2];
  dst = argv[argc - 1];
  
  if(!recursive)
    cp(dst.c_str(), src.c_str());
  else{
    if(src[src.size() - 1] == '/')
      src.resize(src.size() - 1);
    if(dst[dst.size() - 1] == '/')
      dst.resize(dst.size() - 1);
    
    mkdir(dst.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    copy_directory("", pool);
  }

  return 0;
}
