#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include "linked_list.hpp"
#include "c_writer.hpp"
using namespace std;

Linked_List::Linked_List(string input_file){
  this->defined_name = "Score";
  this->cpp_file_name = "score.cpp";
  this->cpp_file.open(this->cpp_file_name);
  this->hpp_file_name = "score.hpp";
  this->hpp_file.open(this->hpp_file_name);
  presets.insert({"int", "score"});
  presets.insert({"char*", "score_name"});
}

Linked_List::~Linked_List()
{
  this->cpp_file.close();
  this->hpp_file.close();
}

void Linked_List::add_cpp_headers(){
  write_line(this->cpp_file, "#include \"" + hpp_file_name + "\"");
  write_new_line(this->cpp_file);
}

vector<string> Linked_List::get_presets(string end){
  vector<string> presets;
  for (auto const& mapping : this->presets){
    presets.push_back(mapping.first + " " + mapping.second + end);
  }
  return presets;
}

void Linked_List::generate_node_struct(){
  write_line(this->hpp_file, "typedef struct " + this->defined_name + "_Node {");
  vector<string> presets_list = this->get_presets(";");
  for(auto const& preset : presets_list){
    write_line(this->hpp_file, preset, 1);
  }
  
  write_line(this->hpp_file, "struct " + this->defined_name + "* next;", 1);
  write_line(this->hpp_file, "}" + this->defined_name + "_Node;");
}

void Linked_List::generate_node_build_function(){
  this->cpp_file << this->defined_name + "_Node* build_" + this->defined_name + "(";
}