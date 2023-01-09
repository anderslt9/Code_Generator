#ifndef PARSE_JSON_H
#define PARSE_JSON_H

#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <string>
#include <any>
#include "error.hpp"
using namespace std;

enum Token{
  OPEN_C_BRACKET,
  CLOSE_C_BRACKET,
  OPEN_BRACKET,
  CLOSE_BRACKET,
  COLON,
  COMMA,
  PAREN,
  VALUE_STRING,
  VALUE_INT,
  VALUE_FLOAT,
  VALUE_SCIENTIFIC,
  VALUE_BOOL,
  VALUE_EMPTY,
  KEY,
};

struct Token_Holder{
  private:
    Token token;
    string value;
  
  public:
    Token_Holder(Token token, string value){
      this->token = token;
      this->value = value;
    }

    Token get_token(){
      return this->token;
    }

    string get_value(){
      return this->value;
    }
};

// class Tokenizer{
//   private:
//     vector<Token_Holder> token_list;

//   public:
//     Tokenizer(){};

//     vector<Token_Holder> get_tokens(){
//       return this->token_list;
//     }

//     bool add_tokens(string to_tokenize);
// };

class Parse_Json{
  private:
    ifstream input_file;
    map<string, any> parsed_object;
    vector<Token_Holder> token_list;

    string parse_string();
    string parse_num();
    string parse_bool();

    void add_token(Token token, string value = "");

  public:
    Parse_Json(string json_file_name);
};

#endif