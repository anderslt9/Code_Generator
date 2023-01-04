#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <memory>
#include <cctype>
#include <regex>
#include "c_writer.hpp"
#include "error.hpp"
using namespace std;

#define BLOCK_INIT_VARS map<string,string> curr_local_vars, int indent, ofstream& out_file
#define BLOCK_INIT_VAR_INPUT this->local_vars, this->indent_level, out_file
#define BLOCK_INIT this->initialize_block(curr_local_vars, indent)
#define BLOCK_START(input) this->start_block(out_file, input)

// ... is arguments to function
#define ADD_CODE_BLOCK(block_type, ...) \
auto function_block = make_shared<block_type>(__VA_ARGS__);\
this->code_blocks.push(function_block);\
this->indent_level++

vector<string> sep_type_from_var(string type_and_var);
bool is_valid_variable(string variable);

enum Block_Type{
  FUNCTION_BUILDER,
  FUNCTION_BLOCK,
  FOR_LOOP_BLOCK,
  WHILE_LOOP_BLOCK,
  IF_STATEMENT_BLOCK,
  ELSE_IF_STATEMENT_BLOCK,
  ELSE_STATEMENT_BLOCK
};

/*
How to hold structure
want: map<string,string>, map<string,map<string,string>>, map<string,map<string,map<string,string>>>, etc
don't need
------------------------
Can instead use map of maps where each outer map is a struct and inner is of form variable: type
map<string,map<string,string>>



*/

class Code_Block {
  protected:
    // local_vars[name] = variable_type
    map<string,string> local_vars;
    int indent_level;
    Block_Type block_type;

    // initializes block with local variables 
    void initialize_block(map<string,string> curr_local_vars, int indent){
      for(auto const& local_var : curr_local_vars){
        this->local_vars[local_var.first] = local_var.second;
      }
      this->indent_level = indent;
    }

    // checks if a variable is currently a local variable or not
    bool is_curr_local_var(string var){
      if(this->local_vars.find(var) == this->local_vars.end()) return false;
      else return true;
    }

    // checks for valid comparison (in other words, it can be reduced to 0 or 1)
    // TODO MASSIVE WORK NEEDED (look at order of precedence table to figure out) (also for variables, make sure they exist in scope)
    bool check_valid_comp(string comparison){
      return true;
      
      // regex or_op("||");
      // smatch sm;
    }

  public:
    Block_Type get_block_type(){
      return this->block_type;
    }
    // Code_Block(map<string,string> curr_local_vars, int indent){
    //   for(auto const& local_var : curr_local_vars){
    //     this->local_vars[local_var.first] = local_var.second;
    //   }
    //   this->indent_level = indent;
    // }

    // starts block of code
    void start_block(ofstream& out_file, string block_start){
      write_line(out_file, block_start + " {", indent_level++);
    };

    // TODO multi-variable declaration

    // adds single variable declaration (initialization optional) 
    bool _add_local_var_(string type, string name, ofstream& out_file, string initialization="") {
      if (this->local_vars.find(name) != this->local_vars.end()) {
        throw invalid_argument(ERROR_VARIABLE_REDECLARATION);
        return false;
      }

      if(!is_valid_variable(name)){
        throw invalid_argument(retrieve_error(ERROR_NOT_VALID_NAME, name));
        return false;
      }


      local_vars[name] = type;

      // checks if variable is initialized and writes line
      if (initialization.length() <= 0) {
        write_line(out_file, type + " " + name + ";", this->indent_level);
      }
      else {
        write_line(out_file, type + " " + name + " = " + initialization + ";", this->indent_level);
      }
      return true;
    }

    bool _reassign_local_var_(string name, string to_set, ofstream& out_file){
      if(!this->is_curr_local_var(name)){
        throw invalid_argument(retrieve_error(ERROR_NOT_LOCAL_VAR, name));
        return false;
      }
      if(!this->check_valid_comp(to_set)){
        throw invalid_argument(ERROR_CONDITION_UNDEFINED + to_set);
        return false;
      }

      write_line(out_file, name + " = " + to_set + ";", this->indent_level);
    }
  
};

// TODO some error handling for variable names and types MAYBE
class Function_Block: public Code_Block{
  public:
    Function_Block(BLOCK_INIT_VARS, string block_start){
      BLOCK_INIT;
      this->block_type = FUNCTION_BLOCK;
      BLOCK_START(block_start);
    }
};

class For_Loop: public Code_Block{
  public:
    /*
    string old_var: variable in form [variable]
    string condition: condition to include (using {old_var} for use of variable)
    string increment: increment part of for loop (using {old_var} for use of variable)
    Example inputs (excluding first 3):
    fish, {old_var} < num_fish, {old_var}++ ---> for(fish; fish < num_fish; fish++) {
    */
    For_Loop(BLOCK_INIT_VARS, string old_var, string condition, string increment) {
      BLOCK_INIT;
      this->block_type = FOR_LOOP_BLOCK;
      string for_loop = "for(";

      // make sure variable exists already
      if(!this->is_curr_local_var(old_var)){
        throw invalid_argument(ERROR_VARIABLE_UNDEFINED + old_var);
        // display_error(ERROR_VARIABLE_UNDEFINED, old_var);
        // return;
      }
      else for_loop += old_var + "; ";

      regex old_var_match("\\{old_var\\}");
      condition = regex_replace(condition, old_var_match, old_var);
      // while (regex_search(condition, old_var_match)) {
      //   out = regex_replace(out, re, "$1b$3");
      // }

      if(!this->check_valid_comp(condition)){
        throw invalid_argument(ERROR_CONDITION_UNDEFINED + condition);
        // display_error(ERROR_CONDITION_UNDEFINED, condition);
        // return;
      }
      else for_loop += condition + "; ";

      
      // TODO check for match with {old_var} and replace
      increment = regex_replace(increment, old_var_match, old_var);
      for_loop += increment + ")";

      BLOCK_START(for_loop);
      // this->start_block(out_file, for_loop);
    };

    /*
    string new_var: variable in form [type] [variable]
    string init_val: value to set new_var to
    string condition: condition to include (using {new_var} for use of variable)
    string increment: increment part of for loop (using {new_var} for use of variable)
    Example inputs (excluding first 3):
    int i, 0, {new_var} < size, {new_var}++ ---> for(int i = 0; i < size; i++) {
    */
    For_Loop(BLOCK_INIT_VARS, string new_var, string init_val, string condition, string increment) {
      BLOCK_INIT;
      this->block_type = FOR_LOOP_BLOCK;

      string for_loop = "for(";
      vector<string> new_var_sep = sep_type_from_var(new_var);
      for_loop += new_var + " = " + init_val + "; ";

      regex new_var_match("\\{new_var\\}");
      condition = regex_replace(condition, new_var_match, new_var_sep[1]);

      if(!this->check_valid_comp(condition)){
        throw invalid_argument(ERROR_CONDITION_UNDEFINED + condition);
        // display_error(ERROR_CONDITION_UNDEFINED, condition);
        // return;
      }
      else for_loop += condition + "; ";

      increment = regex_replace(increment, new_var_match, new_var_sep[1]);
      for_loop += increment + ")";

      BLOCK_START(for_loop);
      // this->start_block(out_file, for_loop);
    };
};

class If_Statement: public Code_Block{
  public:
    If_Statement(BLOCK_INIT_VARS, string condition){
      BLOCK_INIT;
      this->block_type = IF_STATEMENT_BLOCK;
      if(!this->check_valid_comp(condition)){
        throw invalid_argument(ERROR_CONDITION_UNDEFINED + condition);
        return;
      }
      // this->start_block(out_file, "if(" + condition + ")");
      BLOCK_START("if(" + condition + ")");
    }
};

class Else_If_Statement: public Code_Block{
  public:
    Else_If_Statement(BLOCK_INIT_VARS, string condition){
      BLOCK_INIT;
      this->block_type = ELSE_IF_STATEMENT_BLOCK;
      if(!this->check_valid_comp(condition)){
        throw invalid_argument(ERROR_CONDITION_UNDEFINED + condition);
        return;
      }
      // this->start_block(out_file, "if(" + condition + ")");
      BLOCK_START("else if(" + condition + ")");
    }
};

class Else_Statement: public Code_Block{
  public:
    Else_Statement(BLOCK_INIT_VARS){
      BLOCK_INIT;
      this->block_type = ELSE_STATEMENT_BLOCK;
      BLOCK_START("else");
    }
};

class While_Loop: public Code_Block{
  public:
    While_Loop(BLOCK_INIT_VARS, string condition){
      BLOCK_INIT;
      this->block_type = WHILE_LOOP_BLOCK;
      if(!this->check_valid_comp(condition)){
        throw invalid_argument(ERROR_CONDITION_UNDEFINED + condition);
        return;
      }
      // this->start_block(out_file, "if(" + condition + ")");
      BLOCK_START("while(" + condition + ")");
    }
};

class Function_Builder: public Code_Block{
private:
  // map<string,string> args;
  stack<shared_ptr<Code_Block>> code_blocks;
  string name;
  string return_type;
  // int curr_indent;
  // ofstream out_file;

  template <typename T>
  void getArgs(T t){
    vector<string> sep_args = sep_type_from_var(t);
    if(sep_args.size() <= 0) {
      throw invalid_argument(retrieve_error(ERROR_INVALID_ARG, t));
      return;
    }
    this->local_vars[sep_args[1]] = sep_args[0];
  }

  template <typename T, typename... Args>
  void getArgs(T t, Args... args){
    vector<string> sep_args = sep_type_from_var(t);
    if(sep_args.size() <= 0) {
      throw invalid_argument(retrieve_error(ERROR_INVALID_ARG, t));
      return;
    }
    this->local_vars[sep_args[1]] = sep_args[0];
    
    this->getArgs(args...);
  }

public:
  template <typename Name, typename Return_Type, typename Out_File, typename Include_Header_Declaration, typename... Args>
  // incl_head is bool, all other inputs are strings
  Function_Builder(Name name, Return_Type ret, Out_File &out_file, Include_Header_Declaration incl_head, Args... args)
  {
    // TODO deal with header declaration stuff
    TRY_THROW(this->getArgs(args...), )
    //   this->getArgs(args...);
    // }
    // catch(const std::exception& e){
    //   std::cerr << e.what() << endl;
    // }

    // this->getArgs(args...);
    this->name = name;
    this->block_type = FUNCTION_BUILDER;
    // this->out_file.open(out_file);
    this->return_type = ret;
    
    // gets arguments as list
    string args_list = "";
    for(auto const& arg : this->local_vars){
      args_list += arg.second + " " + arg.first + ", ";
      // string arg_name
    }

    // removes extra comma and space
    args_list.pop_back();
    args_list.pop_back();

    string block_start = this->return_type + " " + this->name + "(" + args_list + ")";

    TRY_THROW(ADD_CODE_BLOCK(Function_Block, BLOCK_INIT_VAR_INPUT, block_start),)

    // auto function_block = make_shared<Function_Block>(BLOCK_INIT_VAR_INPUT, block_start);
    // this->code_blocks.push(function_block);

    // this->start_block(out_file, this->return_type + " " + this->name + "(" + args_list + ")");
    // this->code_blocks.top()->indent_level++;
  };

  // ~Function_Builder() {
  //   this->out_file.close();
  // }

  // gets name of function
  string get_name(){
    return this->name;
  }

  // returns true if function is completed
  bool is_finished(){
    if(this->indent_level == 0) return true;
    else return false;
  }

  void end_function(ofstream& out_file){
    while(this->indent_level > 0){
      this->end_block(out_file);
    }
  }

  // adds for loop with new variable
  bool add_for_loop(ofstream &out_file, string new_var, string init_val, string condition, string increment)
  {
    TRY_THROW(ADD_CODE_BLOCK(For_Loop, BLOCK_INIT_VAR_INPUT, new_var, init_val, condition, increment), false);
    return true;
  }

  // adds for loop with predefined condition
  bool add_for_loop(ofstream &out_file, string old_var, string condition, string increment)
  {
    //[x]TODO figure out stuff with TRY
    TRY_THROW(ADD_CODE_BLOCK(For_Loop, BLOCK_INIT_VAR_INPUT, old_var, condition, increment),false);
    return true;
  }

  // adds a new variable
  bool add_new_var(ofstream &out_file, string var, string initialization = ""){
    TRY_THROW(vector<string> sep_args = sep_type_from_var(var);
    if(sep_args.size() <= 0) return false;
    TRY_THROW(this->code_blocks.top()->_add_local_var_(sep_args[0], sep_args[1], out_file, initialization), false), false);
    return true;
  }

  // adds if statement
  bool add_if_statement(ofstream &out_file, string condition){
    TRY_THROW(ADD_CODE_BLOCK(If_Statement, BLOCK_INIT_VAR_INPUT, condition), false);
    return true;
  }

  // adds else if statement
  bool add_else_if_statement(ofstream &out_file, string condition){
    if(this->code_blocks.top()->get_block_type() == IF_STATEMENT_BLOCK || this->code_blocks.top()->get_block_type() == ELSE_IF_STATEMENT_BLOCK){
      this->end_block(out_file);
      TRY_THROW(ADD_CODE_BLOCK(Else_If_Statement, BLOCK_INIT_VAR_INPUT, condition), false);
      return true;
    }
    else{
      throw invalid_argument(ERROR_ELSE_IF);
      cerr << ERROR_ELSE_IF << endl;
      return false;
    }
    
  }

  // adds else statement
  bool add_else_statement(ofstream &out_file){
    if(this->code_blocks.top()->get_block_type() == IF_STATEMENT_BLOCK || this->code_blocks.top()->get_block_type() == ELSE_IF_STATEMENT_BLOCK){
      this->end_block(out_file);
      TRY_THROW(ADD_CODE_BLOCK(Else_Statement, BLOCK_INIT_VAR_INPUT), false);
      return true;
    }
    else{
      throw invalid_argument(ERROR_ELSE);
      cerr << ERROR_ELSE << endl;
      return false;
    }
  }

  // adds while loop
  bool add_while_loop(ofstream &out_file, string condition){
    TRY_THROW(ADD_CODE_BLOCK(While_Loop, BLOCK_INIT_VAR_INPUT, condition), false);
    return true;
  }

  // reassigns existing variable to new value
  bool reassign_var(ofstream &out_file, string var, string to_set){
    TRY_THROW(this->code_blocks.top()->_reassign_local_var_(var, to_set, out_file), false);
    return true;
  }

  // ends block of code
  void end_block(ofstream& out_file);

  // // adds local variable and returns false if variable already exists
  // bool _add_local_var_(string type, string var_name);
};

