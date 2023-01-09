#include <iostream>
#include <any>
#include "linked_list.hpp"
#include "parse_json.hpp"
// #include "c_function_builder.hpp"
// #include "error.hpp"
using namespace std;

int main(int argc, char** argv) {
  if(argc < 2) return EXIT_FAILURE;

  // string next;
  // ifstream input_file("example_linked_list_input.json");
  // while(!input_file.eof()){
  //   cout << (char) input_file.get() << endl;
  //   // input_file >> next;
  //   // cout << next << endl;
  // }

  Parse_Json pj("example_linked_list_input.json");

  // Tokenizer tokenizer;
  // tokenizer.add_tokens("fish man");

  Linked_List list("");
  list.generate_node_struct();
  list.add_cpp_headers();
  
  // ofstream fishy;
  // fishy.open("fishy.c");
  ofstream out_file(argv[1]);
  Function_Builder fb("Potato2", "void", out_file, true, "int ham", "float tortilla");
  TRY_ADD(fb.add_for_loop(out_file, "int i", "0", "{new_var} < ham", "{new_var}++"))
  fb.end_block(out_file);
  TRY_ADD(fb.add_new_var(out_file, "int camel", "0"))
  TRY_ADD(fb.add_if_statement(out_file, "ham > 2"))
  TRY_ADD(fb.reassign_var(out_file, "ham", "7"))
  TRY_ADD(fb.add_else_if_statement(out_file, "ham < 2"))
  TRY_ADD(fb.add_else_statement(out_file))

  fb.end_block(out_file);
  fb.end_block(out_file);
  
  
  if(!fb.is_finished()) fb.end_function(out_file);
  out_file.close();

  return EXIT_SUCCESS;
}