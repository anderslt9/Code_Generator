#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

void write_new_line(ofstream& out_stream, int count = 1);

void write_line(ofstream& out_stream, string line, int num_tabs = 0);

// template <class T>
template <class Out_File, class Num_Tabs, class... Lines>
void write_lines(Out_File &out_file, Num_Tabs num_tabs, vector<Lines...> lines)
{
  for (auto const &elem : lines)
  {
    write_line(out_file, elem, num_tabs);
  }
}