#include <iostream>
#include <fstream>
#include <cstdarg>
#include <vector>
#include "c_writer.hpp"
using namespace std;

void write_new_line(ofstream& out_stream, int count) {
  for(int i = 0; i < count; i++) out_stream << endl;
}

void write_line(ofstream& out_stream, string line, int num_tabs){
  for(int i = 0; i < num_tabs; i++) out_stream << "\t";
  out_stream << line << endl;
}

// TODO Fix this
// template <class Out_File, class Num_Tabs, class ...Lines>
// void write_lines(Out_File& out_file, Num_Tabs num_tabs, Lines... lines)
// {
//   for (auto const& elem : Lines)
//   {
//     write_line(out_file, elem, num_tabs);
//   }
// }
