WINDOWS_FLAGS = -g -O3 -W -Wall -Wunused -Wcast-align -Werror -pedantic -pedantic-errors  -Wfloat-equal -Wpointer-arith -Wwrite-strings -Wno-long-long -Wmissing-declarations
LINUX_FLAGS = -O3 -fsanitize=address -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format
#CFLAGS = $(LINUX_FLAGS)
CFLAGS = -O3 -lm -fsanitize=address -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format

MAX_DEEP = 5
#VALGRIND = valgrind
#CFLAGS = -O3 -W -Wall -Wcast-align -pedantic -pedantic-errors  -Wfloat-equal -Wpointer-arith -Wwrite-strings -Wno-long-long -Wmissing-declarations

a%.out: main%.o
	g++ $(CFLAGS) $^ -o $@

%.o: %.cpp
	 g++ -c $(CFLAGS) $< -o $@

current: a01.out a02.out a03.out a04.out a05.out
other: 
done:

Query.o: Query.cpp Query.h Words_rider.h
make_query_to_base_func.o: make_query_to_base_func.cpp make_query_to_base_func.h

a01.out: main01.o Query.o make_query_to_base_func.o
main01.o: main01.cpp

a02.out: main02.o Query.o make_query_to_base_func.o
main02.o: main02.cpp

a03.out: main03.o Query.o make_query_to_base_func.o
main03.o: main03.cpp

a04.out: main04.o Query.o make_query_to_base_func.o
main04.o: main04.cpp

a05.out: main05.o Query.o make_query_to_base_func.o
main05.o: main05.cpp

clean: 
	rm -f *.out
	rm -f *.o
	rm -f *.exe
	rm -f *summary*
	rm b.txt

test: a01.out a02.out a03.out a04.out a05.out
	./a01.out test_1e2.txt b.txt "abc" " ;-"
	./a01.out test_1e2.txt b.txt "dabc" " ;-"
	./a01.out test_1e2.txt b.txt "dbac" " ;-"
	./a01.out test_1e2.txt b.txt "dbac" " ;-bc"
	./a01.out test_1e2.txt b.txt "dbac" " ;-bcd"
	./a01.out test_1e2.txt b.txt "dbac" " ;-abcd"
	./a01.out test_1e2.txt b.txt "abc" ""
	./a01.out test_1e2.txt b.txt ";" "cd"
	./a01.out test_1e2.txt b.txt ";" "ab"
	./a01.out test_1e4.txt b.txt ";" "ab"
	./a01.out test_1e4.txt b.txt "ab" " -;"
	./a01.out test_1e4.txt b.txt "ab" " -;dc"
	./a01.out test_1e4.txt b.txt "abcd" " -;"
	./a01.out test_1e6.txt b.txt "abcd" " -;"
	./a01.out test_1e6.txt b.txt "; -" " abcd"
	./a01.out test_1e6.txt b.txt ";" " "
	./a01.out test_1e6.txt b.txt ";" " bcd"
	./a02.out test_1e2.txt b.txt ";" "cd"
	./a02.out test_1e2.txt b.txt ";" "cdab "
	./a02.out test_1e2.txt b.txt "; -" "cdab "
	./a02.out test_1e2.txt b.txt "a abc bddabcd bbd" ";- "
	./a02.out test_1e2.txt b.txt " " ";-abcd"
	./a02.out test_1e4.txt b.txt " " ";-abcd"
	./a02.out test_1e4.txt b.txt " a-a;" "abcd"
	./a02.out test_1e4.txt b.txt "abc;" "abcd"
	./a02.out test_1e4.txt b.txt "abc;" "ccdd"
	./a02.out test_1e6.txt b.txt "abc;" "ccdd"
	./a02.out test_1e6.txt b.txt "-a a;" "abcd"
	./a02.out test_1e6.txt b.txt "-a;" "abcd"
	./a03.out test_5.txt b.txt abc " " ABC
	./a03.out test_5.txt b.txt abc " de" ""
	./a03.out test_5.txt b.txt "b bb" " acde" "CC C"
	./a03.out test_5.txt b.txt "b bb" " acde" "AAA"
	./a03.out test_5.txt b.txt "b bb" " acde" "A C"
	./a04.out test_1e2.txt b.txt "a" " ;-" "<"
	./a04.out test_1e2.txt b.txt "a" " ;-" ">"
	./a04.out test_1e4.txt b.txt "dddd" " ;-" ">="
	./a04.out test_1e4.txt b.txt "dddd" " ;-" "<"
	./a04.out test_1e4.txt b.txt "dddd" " ;-" "="
	./a04.out test_1e4.txt b.txt " " " ;-" "="
	./a04.out test_1e4.txt b.txt " " "abcd" "="
	./a04.out test_1e4.txt b.txt " " "abcd" "<>"
	./a04.out test_1e6.txt b.txt "bbb" " -;a" ">"
	./a04.out test_1e6.txt b.txt "bbb" " -;a" "<"
	./a04.out test_1e6.txt b.txt "o" " -;" "<>"
	./a04.out test_1e6.txt b.txt "abcd" ";" ">"
	./a04.out test_1e6.txt b.txt "abcd" ";" "<"
	./a04.out test_1e6.txt b.txt "aaaaaaa" " -;" "<"
	./a04.out test_1e6.txt b.txt "a" " -;" "<="
	./a05.out test_1e2.txt b.txt "a" " -;" ">"
	./a05.out test_1e2.txt b.txt "a" " -;" "<"
	./a05.out test_1e2.txt b.txt "o" " -;" "<>"
	./a05.out test_1e2.txt b.txt "aab" " -;" ">="
	./a05.out test_1e4.txt b.txt "" " -;" ">="
	./a05.out test_1e4.txt b.txt "bcc" " -;a" "<"
	./a05.out test_1e4.txt b.txt "bcc" " -;a" ">"
	./a05.out test_1e4.txt b.txt "abcd" " -;bd" "<="
	./a05.out test_1e4.txt b.txt "aabccc" " -;d" "<"
	./a05.out test_1e6.txt b.txt "aa" " -;bcd" "<"
	./a05.out test_1e6.txt b.txt "aa" " -;bcd" ">"
	./a05.out test_1e6.txt b.txt "a" " -;" ">"
	./a05.out test_1e6.txt b.txt "b" " -;" ">"
	./a05.out test_1e6.txt b.txt "d" " -;" ">"
	./a05.out test_1e6.txt b.txt "d" " -;abc" ">"
	./a05.out test_1e6.txt b.txt " " "-;ab" "<="
	./a05.out test_1e6.txt b.txt " " "-;ab" "<>"
