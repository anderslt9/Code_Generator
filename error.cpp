#include <iostream>
#include <regex>
#include "error.hpp"
// #include <cctype>
using namespace std;

string retrieve_error(string error_message, string substitution){
  regex replace_reg("\\{arg\\}");
  if(substitution.length() <= 0) return error_message;
  else return regex_replace(error_message, replace_reg, substitution);
}

string retrieve_error(string error_message, char substitution){
  string sub = substitution + "";
  regex replace_reg("\\{arg\\}");
  if(sub.length() <= 0) return error_message;
  else return regex_replace(error_message, replace_reg, sub);
}