#ifndef LINKED_LIST_H
#define LINKEDLIST_H


#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include "c_function_builder.hpp"
using namespace std;

class Linked_List {
private:
  ofstream cpp_file;
  string cpp_file_name;
  ofstream hpp_file;
  string hpp_file_name;
  string defined_name;
  map<string, string> presets;


public:
  Linked_List(string input_file);
  
  ~Linked_List();

  void add_cpp_headers();

  // returns presets as vector list with each item ending with given end
  vector<string> get_presets(string end);

  void generate_node_struct();

  void generate_node_build_function();

  void generate_node_free_function();

  void generate_list_free_function();
};

#endif