WINDOWS_FLAGS = -g -O0 -W -Wall -Wunused -Wcast-align -Werror -pedantic -pedantic-errors  -Wfloat-equal -Wpointer-arith -Wwrite-strings -Wno-long-long -Wmissing-declarations
FAST_FLAGS = -O3
LINUX_FLAGS = -O3 -fsanitize=address -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format
PROF_FLAGS = -O3 -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format -pg

TEST_PATH = tests/
CFLAGS = $(FAST_FLAGS)
#CFLAGS = -O3 -lm -fsanitize=address -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format


#VALGRIND = valgrind
#CFLAGS = -O3 -W -Wall -Wcast-align -pedantic -pedantic-errors  -Wfloat-equal -Wpointer-arith -Wwrite-strings -Wno-long-long -Wmissing-declarations

current: a.out
other: avl_test.out

avl_test.out: avl_main.o  record.o
	g++ $(CFLAGS) $^ -o $@
	rm -f *.o
avl_main.o: avl_main.cpp

main.o: main.cpp
record.o: record.cpp
command.o: command.cpp
database_command.o: database_command.cpp

a.out: main.o record.o command.o database_command.o
	g++ $(CFLAGS) $^ -o $@
	rm -f *.o
%.o: %.cpp
	g++ -c $(CFLAGS) $< -o $@

test: a.out
	cat $(TEST_PATH)commands.txt | ./a.out $(TEST_PATH)test_1e2.txt > $(TEST_PATH)myres1e2.txt
	cat $(TEST_PATH)commands.txt | ./a.out $(TEST_PATH)test_1e4.txt > $(TEST_PATH)myres1e4.txt
#	cat $(TEST_PATH)commands.txt | ./a.out $(TEST_PATH)test_1e6.txt > $(TEST_PATH)myres1e6.txt
	

check_memory: a.out
	valgrind cat $(TEST_PATH)commands.txt | ./a.out $(TEST_PATH)test_1e2.txt > $(TEST_PATH)myres1e2.txt

clean: 
	rm -f *.out
	rm -f *.o
	rm -f *.exe
	rm -f myres*.txt
