WINDOWS_FLAGS = -g -O0 -W -Wall -Wunused -Wcast-align -Werror -pedantic -pedantic-errors  -Wfloat-equal -Wpointer-arith -Wwrite-strings -Wno-long-long -Wmissing-declarations

LINUX_FLAGS = -O3 -fsanitize=address -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format

CFLAGS = $(LINUX_FLAGS)
#CFLAGS = -O3 -lm -fsanitize=address -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format


#VALGRIND = valgrind
#CFLAGS = -O3 -W -Wall -Wcast-align -pedantic -pedantic-errors  -Wfloat-equal -Wpointer-arith -Wwrite-strings -Wno-long-long -Wmissing-declarations

current: a.out

main.o: main.cpp
record.o: record.cpp
command.o: command.cpp

a.out: main.o record.o command.o
	g++ $(CFLAGS) $^ -o $@
%.o: %.cpp
	 g++ -c $(CFLAGS) $< -o $@

test:
	cat commands.txt | ./a.out test_1e2.txt > "myres1e2.txt"
	cat commands.txt | ./a.out test_1e4.txt > "myres1e4.txt"
	cat commands.txt | ./a.out test_1e6.txt > "myres1e6.txt"

clean: 
	rm -f *.out
	rm -f *.o
	rm -f *.exe
	rm -f myres*.txt
