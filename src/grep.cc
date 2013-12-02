#include <iostream>
#include <fstream>
#include <getopt.h>
#include <string>
#include "ThreadPool.h"
#include <boost/regex.hpp>
#include <stdio.h>

static bool invert = false;
static bool icase = false;
static char* filename = 0;
static

static struct option long_options[] = {
  {"--file",          required_argument,  0,  'f'},
  {"--invert-match",  no_argument,        0,  'v'},
  {"--ignore-case",   no_argument,        0,  'i'},
  {0,                 0,                  0,  0}
};

void usage(){
  printf("Usage : \n");
}

void init_args(int argc, char* argv[]){
  int opt_index = 0;
  int opt;
  
  while((opt = getopt_long(argc, argv, "f:iv", long_options, &opt_index)) != -1){
    switch(opt){
      case 0:
        fprintf(stderr, "get_opt bug?\n");
        break;
      
      case 'f':
        filename = optarg;
        break;
      
      case 'v':
        invert = true;
        break;

      case 'i':
        icase = true;
        break;

      case '?':
        usage();
        break;

      default:
        printf("%s\n", optarg);
        fprintf(stderr, "Invalid arguments (%c)\n", opt);
        usage();
    }
  }
}

int main(int argc, char *argv[]){
  
  init_args(argc, argv);
  ThreadPool t(4);
  
  std::string pattern = argv[argc - 1];
  
  if(filename)
    in = new ifstream(filename);
  else 
    in = new istream;

  /*
  boost::regex e("heLLo", boost::regex_constants::grep | boost::regex::icase);
  std::string content = "hello world!";

  cout<<regex_search(content, e)<<std::endl;
  */
  
  return 0;
}
