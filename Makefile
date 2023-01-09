EXEC = code_gen
G++ = g++ -Wall -Wextra -g -std=c++17
OBJS = main.cpp linked_list.cpp c_writer.cpp c_function_builder.cpp error.cpp parse_json.cpp
HDRS = linked_list.hpp c_writer.hpp c_function_builder.hpp error.hpp parse_json.hpp
TEST1_C = test_file_1.c
TEST2_C = test_file_2.c
TEST3_C = test_file_3.c
TEST4_C = test_file_4.c
TEST_FILES = test_file_*.c

$(EXEC): $(OBJS) $(HDRS)
	$(G++) $(OBJS) -o $(EXEC)

test1: $(OBJS) $(HDRS) $(EXEC)
	./$(EXEC) $(TEST1_C)
	
memory: $(OBJS) $(HDRS) $(EXEC)
	valgrind ./$(EXEC) $(TEST1_C)

clean:
	rm -f $(EXEC)
	rm -f *.gch
	rm -f $(TEST_FILES)