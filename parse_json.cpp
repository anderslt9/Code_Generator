#include <iostream>
#include <fstream>
#include <map>
#include <regex>
#include <deque>
#include <set>
#include "parse_json.hpp"
using namespace std;

/*
General Idea:
Use deque to represent key orders
Example:
1. start with empty deque
2. append "name" to map as map["name"] = "Score"
3. push "structs" onto deque
4. read empty array as empty map as map["structs"] = map
5. pop "structs" off deque
6. push "presets" onto deque
7. push "0" onto deque (arrays become indexed like this) (also potentially just use vector)
8. append "data type" to map as map["presets"]["0"]["data_type"] = "int"
9. append "variable_name" to map as map["presets"]["0"]["variable_name"] = "score"
10. pop "0" off stack
11. pop "presets" off stack
12. return
{
  "name": "Score",
  "structs": [],
  "presets": [
    {
      "data_type": "int",
      "variable_name": "score"
    }
  ],
}
*/

Parse_Json::Parse_Json(string json_file_name){
  this->input_file.open(json_file_name);
  while(this->input_file.peek() != EOF){
    char first_token = this->input_file.peek();
    switch (first_token)
    {
    case '\n':
    case ' ':
      this->input_file.get();
      break;
    case '{':
      this->input_file.get();
      this->add_token(OPEN_C_BRACKET);
      break;
    case '}':
      this->input_file.get();
      this->add_token(CLOSE_C_BRACKET);
      break;
    case '[':
      this->input_file.get();
      this->add_token(OPEN_BRACKET);
      break;
    case ']':
      this->input_file.get();
      this->add_token(CLOSE_BRACKET);
      break;
    case ':':
      this->input_file.get();
      this->add_token(COLON);
      break;
    case ',':
      this->input_file.get();
      this->add_token(COMMA);
      break;
    case '\"': {
      // check for string
      string parsed_string;
      TRY_THROW(
        this->add_token(VALUE_STRING, this->parse_string()),);

      break;
    }
    case 't':
    case 'f':
      // check for true or false
      TRY_THROW(this->add_token(VALUE_BOOL, this->parse_bool()),);
      break;
    case 'n': {
      string null_check = "null";
      for(unsigned int i = 0; i < null_check.length(); i++){
        char next_char = this->input_file.get();
        if(next_char != null_check[i]) {
          THROW_ERROR(ERROR_INVALID_CHAR, next_char)
          return;
        }
      }
      this->add_token(VALUE_EMPTY);
      break;
    }

    default:
      regex match_int("^(\\-{0,1}|\\+{0,1})([1-9][0-9]*|0)$");
      regex match_dec("^(\\-{0,1}|\\+{0,1})[0-9]+\\.[0-9]+$");
      regex match_sci("^(\\-{0,1}|\\+{0,1})[0-9]+(e|E)(\\-{0,1}|\\+{0,1})([[0-9]+)$");

      string matched_num;
      TRY_THROW(matched_num = this->parse_num(),)
      if(regex_match(matched_num, match_int)) this->add_token(VALUE_INT, matched_num);
      else if(regex_match(matched_num, match_dec)) this->add_token(VALUE_FLOAT, matched_num);
      else if(regex_match(matched_num, match_sci)) this->add_token(VALUE_SCIENTIFIC, matched_num);
      else {
        THROW_ERROR(ERROR_INVALID_NUMBER, matched_num)
        return;
      }
      break;
    }
  }

  // replace "string": "colon" with "key"
  unsigned int el = 0;
  while(el < this->token_list.size()){
    if(token_list[el].get_token() == COLON){
      if(el == 0 || token_list[el - 1].get_token() != VALUE_STRING){
        THROW_ERROR(ERROR_INVALID_CHAR, ':')
        return;
      }
      else{
        string key = token_list[el - 1].get_value();
        token_list.erase(token_list.begin() + el--);
        token_list.erase(token_list.begin() + el);
        token_list.emplace(token_list.begin() + el++, Token_Holder(KEY,key));
      }
    }
    else el++;
  }
}

void Parse_Json::add_token(Token token, string value){
  if(value == ""){
    switch (token)
    {
    case OPEN_C_BRACKET:
      value = "{";
      break;
    case CLOSE_C_BRACKET:
      value = "}";
      break;
    case OPEN_BRACKET:
      value = "[";
      break;
    case CLOSE_BRACKET:
      value = "]";
      break;
    case COLON:
      value = ":";
      break;
    case COMMA:
      value = ",";
      break;
    }
  }
  
  Token_Holder tok(token, value);
  this->token_list.push_back(tok);
}

string Parse_Json::parse_string(){
  string str_parsed = "";
  char element;
  char first_char = this->input_file.get();
  if(first_char != '\"'){
    THROW_ERROR(ERROR_INVALID_CHAR, first_char)
    return "";
  }
  while(!this->input_file.eof()) {
    element = this->input_file.get();
    if(element == '"'){
      // this->input_file.get();
      break;
    }
    else if(element != '\\'){
      str_parsed += element;
    }
    else{
      if(this->input_file.peek() == EOF){
        THROW_ERROR(ERROR_INVALID_JSON_SLASH,"")
        return "";
      }
      switch (this->input_file.peek())
      {
      case '\"':
        str_parsed += "\"";
        this->input_file.get();
        break;
      case '\\':
        str_parsed += "\\";
        this->input_file.get();
        break;
      case '/':
        str_parsed += "/";
        this->input_file.get();
        break;
      case 'b':
        str_parsed += "\b";
        this->input_file.get();
        break;
      case 'f':
        str_parsed += "\f";
        this->input_file.get();
        break;
      case 'n':
        str_parsed += "\n";
        this->input_file.get();
        break;
      case 'r':
        str_parsed += "\r";
        this->input_file.get();
        break;
      case 't':
        str_parsed += "\t";
        this->input_file.get();
        break;
      case 'u': {
        // str_parsed += "\"";
        // if(i > string_to_parse.length() - 5){
        //   THROW_ERROR(ERROR_INVALID_SLASH_U,"")
        //   return "";
        // }
        string temp_string = "";
        this->input_file.get();
        for(int i = 0; i < 4; i++){
          if(this->input_file.peek() == EOF){
            THROW_ERROR(ERROR_INVALID_SLASH_U, "")
            return "";
          }
          temp_string += this->input_file.get();
        }
        
        regex hex_vals("[a-fA-F0-9]");
        if(!regex_match(temp_string, hex_vals)){
          THROW_ERROR(ERROR_INVALID_SLASH_U,"")
          return "";
        }
        
        str_parsed += "0x" + temp_string;
        break;
      }
      
      default:
        THROW_ERROR(ERROR_INVALID_SLASH_CHAR, this->input_file.peek())
        return "";
        break;
      }
    }
  }

  if(this->input_file.eof()){
    THROW_ERROR(ERROR_NO_CLOSING_QUOTES,"")
    return "";
  }
  
  return str_parsed;
}

string Parse_Json::parse_bool(){
  string bool_to_check;
  if(this->input_file.peek() == 't'){
    bool_to_check = "true";
  }
  else if(this->input_file.peek() == 'f') bool_to_check = "false";

  else {
    THROW_ERROR(ERROR_INVALID_CHAR, this->input_file.peek());
    return "";
  }

  for(unsigned int i = 0; i < bool_to_check.length(); i++){
    char new_char = this->input_file.get();
    if(new_char != bool_to_check[i]){
      THROW_ERROR(ERROR_INVALID_CHAR, new_char);
      return "";
    }
  }
  return bool_to_check;
}

string Parse_Json::parse_num(){
  // regex match_int("^(\\-{0,1}|\\+{0,1})([1-9][0-9]*|0)$");
  // regex match_dec("^(\\-{0,1}|\\+{0,1})[0-9]+\\.[0-9]+$");
  // regex match_sci("^(\\-{0,1}|\\+{0,1})[0-9]+(e|E)(\\-{0,1}|\\+{0,1})([1-9][0-9]+|0)$");

  // regex match_valid_char("^[0-9\\+\\-eE\\.]$");
  char temp_arr[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '.', '+', '-', 'e', 'E'};
  set<char> valid_chars(temp_arr, temp_arr + 16);


  string num_parsed = "";
  while(!this->input_file.eof() && valid_chars.count(this->input_file.peek())){
    num_parsed += this->input_file.get();
  }
  if(this->input_file.eof()){
    THROW_ERROR(ERROR_END_OF_FILE,"")
    return "";
  }

  return num_parsed;
}
  // cout << (char)this->input_file.peek() << endl;
  // while(!this->input_file.eof() && regex_match("" + this->input_file.peek(), match_valid_char)){
  //   num_parsed += this->input_file.get();
  // }
  // if(this->input_file.eof()){
  //   THROW_ERROR(ERROR_END_OF_FILE,"")
  //   return "";
  // }




// bool Tokenizer::add_tokens(string to_tokenize){
//   cout << "Before: " + to_tokenize << endl;
//   // cout << "((?<!(\"([^\"]|\\\"{2}|[\\\"])*))(?!\")\\s+)" << endl;
//   // regex space_to_remove("((?<!(\"([^\"]|\\\"{2}|[\\\"])*))(?!\")\\s+)");
//   // to_tokenize = regex_replace(to_tokenize, space_to_remove, "");
//   // cout << "After: " + to_tokenize << endl;
//   unsigned int string_pointer_loc = 0;

//   // while(string_pointer_loc < to_tokenize.length()){
//   //   char first_token = to_tokenize[string_pointer_loc];
//   //   switch (first_token)
//   //   {
//   //   case ' ':
//   //     string_pointer_loc++;
//   //     break;
//   //   case '{':
//   //     Token_Holder tok(OPEN_C_BRACKET, '{');
//   //     this->token_list.push(tok);
//   //     string_pointer_loc++;
//   //     break;
//   //   case '}':
//   //     Token_Holder tok(CLOSE_C_BRACKET, '}');
//   //     this->token_list.push(tok);
//   //     string_pointer_loc++;
//   //     break;
//   //   case '[':
//   //     Token_Holder tok(OPEN_BRACKET, '[');
//   //     this->token_list.push(tok);
//   //     string_pointer_loc++;
//   //     break;
//   //   case ']':
//   //     Token_Holder tok(CLOSE_BRACKET, ']');
//   //     this->token_list.push(tok);
//   //     string_pointer_loc++;
//   //     break;
//   //   case ':':
//   //     Token_Holder tok(COLON, ':');
//   //     this->token_list.push(tok);
//   //     string_pointer_loc++;
//   //     break;
//   //   case ',':
//   //     Token_Holder tok(COMMA, ',');
//   //     this->token_list.push(tok);
//   //     string_pointer_loc++;
//   //     break;
//   //   case '\"':
//   //     /* code */
//   //     break;
    
//   //   default:
//   //     break;
//   //   }
//   // }





//   return true;
// }



