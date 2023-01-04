#include <iostream>
#include <fstream>
#include <cstdarg>
#include <vector>
#include <regex>
#include "c_function_builder.hpp"
// #include "c_writer.hpp"
using namespace std;

// void Function_Builder::start_block(string block_start, ofstream& out_file){
//   write_line(out_file, block_start + " {", this->indent_level);
//   this->indent_level += 1;
// }

vector<string> sep_type_from_var(string type_and_var) {
  vector<string> separated_variable;
  unsigned int last_space = type_and_var.length();
  for(unsigned int i = 0; i < type_and_var.length() - 1; i++){
    if(type_and_var[i] == ' '){
      last_space = i;
    }
  }
  if(last_space >= type_and_var.length()) return separated_variable;
  else if(!is_valid_variable(type_and_var.substr(last_space + 1))){
    throw invalid_argument(retrieve_error(ERROR_NOT_VALID_NAME, type_and_var.substr(last_space + 1)));
    return separated_variable;
  } 
  separated_variable.push_back(type_and_var.substr(0, last_space));
  separated_variable.push_back(type_and_var.substr(last_space + 1));
  
  return separated_variable;
}

bool is_valid_variable(string variable){
  // removes *'s if pointer type
  while(variable[0] == '*'){
    variable = variable.substr(1);
  }
  
  regex var("^[A-Za-z][A-Za-z0-9_]*$");
  return regex_match(variable, var);
}

void Function_Builder::end_block(ofstream& out_file){
  this->indent_level -= 1;
  write_line(out_file, "}", this->indent_level);
  this->code_blocks.pop();
}
