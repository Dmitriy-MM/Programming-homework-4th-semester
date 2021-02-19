WINDOWS_FLAGS = -g -O3 -W -Wall -Wunused -Wcast-align -Werror -pedantic -pedantic-errors  -Wfloat-equal -Wpointer-arith -Wwrite-strings -Wno-long-long -Wmissing-declarations
LINUX_FLAGS = -O3 -fsanitize=address -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format
CFLAGS = $(WINDOWS_FLAGS)

MAX_DEEP = 5
#VALGRIND = valgrind
#CFLAGS = -O3 -W -Wall -Wcast-align -pedantic -pedantic-errors  -Wfloat-equal -Wpointer-arith -Wwrite-strings -Wno-long-long -Wmissing-declarations

a%.out: main%.o
	g++ $(CFLAGS) $^ -o $@

%.o: %.cpp
	 g++ -c $(CFLAGS) $< -o $@

current: a03.out 
other: a02.out a01.out a04.out a05.out
done:

query.o: Query.cpp Query.h Words_rider.h
make_query_to_base_func.o: make_query_to_base_func.cpp make_query_to_base_func.h

a01.out: main01.o query.o make_query_to_base_func.o
main01.o: main01.cpp

a02.out: main02.o query.o make_query_to_base_func.o
main02.o: main02.cpp

a03.out: main03.o query.o make_query_to_base_func.o
main03.o: main03.cpp

a04.out: main04.o query.o make_query_to_base_func.o
main04.o: main04.cpp

a05.out: main05.o query.o make_query_to_base_func.o
main05.o: main05.cpp

clean: 
	rm -f *.out
	rm -f *.o
	rm -f *.exe
	