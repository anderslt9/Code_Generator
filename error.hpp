#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include <regex>
#include <cctype>
using namespace std;

#define TRY(input_text, return_val) \
  try{ \
    input_text; \
  } \
  catch(const std::exception& e){ \
    std::cerr << e.what() << endl; \
    return return_val; \
  }

#define TRY_THROW(input_text, return_val) \
  try{ \
    input_text; \
  } \
  catch(const std::exception& e){ \
    throw invalid_argument(e.what()); \
    return return_val; \
  }

#define TRY_ADD(input_text) \
  try{ \
    input_text; \
  } \
  catch(const std::exception& e){ \
    std::cerr << e.what() << endl; \
    return EXIT_FAILURE; \
  }

#define THROW_ERROR(error_type, substitution) throw invalid_argument(retrieve_error(error_type, substitution));
// #define THROW_ERROR(error_type) throw invalid_argument(retrieve_error(error_type));

#define ERROR_VARIABLE_REDECLARATION "Error: variables can't be redeclared"
#define ERROR_VARIABLE_UNDEFINED "Error: there are no variables named "
#define ERROR_CONDITION_UNDEFINED "Error: condition given is unspecified\n"
#define ERROR_INVALID_ARG "Error: argument {arg} is undefined" 
#define ERROR_ELSE_IF "Error: else if statements must be declared after if or else if statement"
#define ERROR_ELSE "Error: else statements must be declared after if or else if statement"
#define ERROR_NOT_LOCAL_VAR "Error: variable {arg} is not a current local variable"
#define ERROR_NOT_VALID_NAME "Error: {arg} is not a valid variable name"
#define ERROR_INVALID_JSON_SLASH "Error: invalid use of \\ symbol"
#define ERROR_INVALID_SLASH_U "Error: \\u must be immediately followed by 4 hexadecimal digits"
#define ERROR_INVALID_SLASH_CHAR "Error: {arg} is an invalid character with use of escape character \\"
#define ERROR_INVALID_CHAR "Error: character '{arg}' is invalid here"
#define ERROR_END_OF_FILE "Error: end of file reached before code blocks terminated"
#define ERROR_INVALID_NUMBER "Error: {arg} is not a valid JSON value"
#define ERROR_NO_CLOSING_QUOTES "Error: no closing quotes present"


// TODO maybe implement regex with replacing something in error_message with additional_message

// void display_error(string error_message, string additional_message = ""){
//   cerr << error_message + additional_message << endl;
// }

string retrieve_error(string error_message, string substitution = "");
string retrieve_error(string error_message, char substitution = ' ');

#endif