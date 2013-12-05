#include <iostream>
#include <string>
#include "ThreadPool.h"
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <vector>
#include <map>
#include <fstream>
#include "util.h"

static std::istream *in;
static char* filename = 0;
static std::vector<std::string> commands;

enum{SYNTAX_ERROR = 1, INVALID_ID};

static struct option long_options[] = {
  {"--file",          required_argument,  0,  'f'},
  {0,                 0,                  0,  0}
};

void init_args(int argc, char* argv[]){
  int opt_index = 0;
  int opt;
  
  while((opt = getopt_long(argc, argv, "f:", long_options, &opt_index)) != -1){
    switch(opt){
      case 0:
        fprintf(stderr, "get_opt bug?\n");
        break;
      
      case 'f':
        filename = optarg;
        break;
    }
  }
}

void parse_error(int err_id){ 
  switch(err_id){
    case SYNTAX_ERROR:
      std::cerr<<"Syntax error in the input file, please have a check."<<std::endl;
      break;

    case INVALID_ID:
      std::cerr<<"Invalid id."<<std::endl;
      break;
  }
  exit(-1);
}

int get_lineinfo(std::string& line, int &dep_id, int& id){
  int colon_pos;
  dep_id = -1;
  id = -1;

  colon_pos = line.find(":");
  
  if(colon_pos == -1)
    return 0;
  
  if(line[0] != '#' && line[0] != '-')
    return 0;

  std::string line_tag = line.substr(0, colon_pos);
  if(line_tag[0] == '#')
    id = atoi(line_tag.substr(1, colon_pos).c_str());
  
  int dash_pos = line_tag.find("-");
  if(dash_pos != -1)
    dep_id = atoi(line_tag.substr(dash_pos + 1, colon_pos).c_str());
  
  line = line.substr(colon_pos + 1, line.size());

  return 0;
}

void build_commands_chain(){
  //map id of command to the index of it in commands chain.
  std::map<int, int> id2idx; 
  std::string line;
  int dep_id, id;

  while(std::getline(*in, line)){ 
    get_lineinfo(line, dep_id, id);

    if(id == -1 && dep_id == -1)
      commands.push_back(line);
  
    if(id != -1 && id2idx.find(id) != id2idx.end())
      parse_error(INVALID_ID);
    
    if(dep_id != -1 && id2idx.find(dep_id) == id2idx.end())
      parse_error(INVALID_ID);
    
    if(id != -1 && dep_id != -1){
      int dep_no = id2idx[dep_id];
      id2idx[id] = dep_no;
      commands[dep_no] = commands[dep_no] + ";" + line;
    }
    else if(dep_id != -1){
      int dep_no = id2idx[dep_id];
      commands[dep_no] = commands[dep_no] + ";" + line;
    }
    else if(id != -1){
      id2idx[id] = commands.size();
      commands.push_back(line);
    }
  }
}        

void print_commands_chain(){
  for(int i = 0; i < commands.size(); ++i)
    std::cout<<commands[i]<<std::endl;
}


void execute_commands(std::string command){
  system(command.c_str());
}


void run(ThreadPool &pool){
  build_commands_chain();
  for(int i = 0; i < commands.size(); ++i)
    pool.enqueue(execute_commands, commands[i]);
}


int main(int argc, char *argv[]){
  init_args(argc, argv);
  
  int n_threads = get_nthreads();
  ThreadPool pool(n_threads, 100);
  
  if(filename == 0)
    in = &std::cin;
  else{
    in = new std::ifstream(filename);
    if(!((std::ifstream*)in)->is_open()){
      fprintf(stderr, "Can't open file %s\n", filename);
      exit(-1);
    }
  }

  run(pool);

  return 0;
}
