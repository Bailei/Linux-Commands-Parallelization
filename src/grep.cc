#include <iostream>
#include <fstream>
#include <getopt.h>
#include <string>
#include "ThreadPool.h"
#include <boost/regex.hpp>
#include <stdio.h>
#include <vector>
#include <sstream>

static const uint32_t nlines_per_worker = 0xf;
static bool invert = false;
static bool icase = false;
static char* filename = 0;
static std::istream *in;
static boost::regex reg_exp;

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

std::string print(std::string content){
  std::istringstream is(content);
  std::string result;
  std::string line;

  while(std::getline(is, line)){
    if(regex_search(line, reg_exp))
      result += line + "\n";
  }

  return result;
}

void run(ThreadPool &pool){ 
  std::string line;
  std::string lines;
  std::vector<std::future<std::string> > results;
  uint32_t n_lines = 0;

  while(std::getline(*in, line)){
    lines += line + "\n";
    n_lines++;

    if((n_lines & nlines_per_worker) == 0){
     results.push_back(pool.enqueue(print, lines)); 
     lines.clear();
    }
  }
  
  if(!lines.empty())     
    results.push_back(pool.enqueue(print, lines)); 
  
  for(int i = 0; i < results.size(); ++i)
    std::cout<<results[i].get();
}


int main(int argc, char *argv[]){
  
  init_args(argc, argv);
  ThreadPool pool(4);
  
  std::string pattern = argv[argc - 1];
  
  if(filename){
    in = new std::ifstream(filename);
    if(!((std::ifstream*)in)->is_open()){
      std::cerr<<"Can't open file "<<filename<<std::endl;
      exit(-1); 
    }
  }
  else
    in = &std::cin;

  reg_exp.assign(pattern.c_str(), boost::regex_constants::grep);
  
  run(pool);
  
  return 0;
}
