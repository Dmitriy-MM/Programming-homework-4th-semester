WINDOWS_FLAGS = -g -O3 -W -Wall -Wunused -Wcast-align -Werror -pedantic -pedantic-errors  -Wfloat-equal -Wpointer-arith -Wwrite-strings -Wno-long-long -Wmissing-declarations

LINUX_FLAGS = -O3 -fsanitize=address -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format

CFLAGS = $(LINUX_FLAGS)
#CFLAGS = -O3 -lm -fsanitize=address -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format

COMMON_REQUIERIES = Query.o make_query_to_base_func.o

MAX_DEEP = 5
#VALGRIND = valgrind
#CFLAGS = -O3 -W -Wall -Wcast-align -pedantic -pedantic-errors  -Wfloat-equal -Wpointer-arith -Wwrite-strings -Wno-long-long -Wmissing-declarations

a%.out: main%.o
	g++ $(CFLAGS) $^ -o $@

%.o: %.cpp
	 g++ -c $(CFLAGS) $< -o $@

current: a01.out a02.out a03.out a04.out a05.out a06.out
other: 
done:

Query.o: Query.cpp Query.h Words_rider.h Word_regexp.h
make_query_to_base_func.o: make_query_to_base_func.cpp make_query_to_base_func.h

a01.out: main01.o $(COMMON_REQUIERIES)
main01.o: main01.cpp

a02.out: main02.o $(COMMON_REQUIERIES)
main02.o: main02.cpp

a03.out: main03.o $(COMMON_REQUIERIES)
main03.o: main03.cpp

a04.out: main04.o $(COMMON_REQUIERIES)
main04.o: main04.cpp

a05.out: main05.o $(COMMON_REQUIERIES)
main05.o: main05.cpp

a06.out: main06.o $(COMMON_REQUIERIES)
main06.o: main06.cpp

clean: 
	rm -f *.out
	rm -f *.o
	rm -f *.exe
	rm -f *summary*.txt
	rm -f b.txt


test: a01.out a02.out a03.out a04.out a05.out a06.out
	./a01.out test_1e2.txt b.txt "ab_" " ;e"
	./a01.out test_1e2.txt b.txt "_____" " ;abc"
	./a01.out test_1e2.txt b.txt "__b_" " ;aec"
	./a01.out test_1e2.txt b.txt "_ab__b" " ;dec"
	./a01.out test_1e4.txt b.txt "_" " ;a"
	./a01.out test_1e4.txt b.txt "_ccc__" " ;aed"
	./a01.out test_1e4.txt b.txt "a__" " ;bed"
	./a01.out test_1e4.txt b.txt "_d_d_d__" " ;bea"
	./a01.out test_1e6.txt b.txt "__" " ;bea"
	./a01.out test_1e6.txt b.txt "_ad_" " ;bec"
	./a01.out test_1e6.txt b.txt "_________" " ;"
	./a01.out test_1e6.txt b.txt "_b_" " ;a"
	./a02.out test_1e2.txt b.txt "__ ___" " "
	./a02.out test_1e2.txt b.txt "____ ___ ___" "; "
	./a02.out test_1e2.txt b.txt "__a____" "; "
	./a02.out test_1e4.txt b.txt "_ ____" "; "
	./a02.out test_1e4.txt b.txt "____ _c_c" "; ab"
	./a02.out test_1e4.txt b.txt "__ _ _ __ ___" "; ab"
	./a02.out test_1e6.txt b.txt "__a_ a__" "; cd"
	./a02.out test_1e6.txt b.txt "__ ___c a__  __   d __" "; cd"
	./a02.out test_1e6.txt b.txt "aaa___a a__b" "; cd"
	./a02.out test_1e6.txt b.txt "_____ _c__  __a__" "; bd"
	./a03.out test_1e2.txt b.txt "%" " ;e"
	./a03.out test_1e2.txt b.txt "%ab%" " ;d"
	./a03.out test_1e2.txt b.txt "cc%ab%" " ;e"
	./a03.out test_1e2.txt b.txt "ab%c" " ;d"
	./a03.out test_1e4.txt b.txt "aaaaa%" " ;d"
	./a03.out test_1e4.txt b.txt "a%d%a%d" " ;bc"
	./a03.out test_1e4.txt b.txt "aaaaa%dddd%aa%" " ;bc"
	./a03.out test_1e4.txt b.txt "%%%%%d%%%%%" " ;bc"
	./a03.out test_1e6.txt b.txt "%%c%d%%%d%" " ;ba"
	./a03.out test_1e6.txt b.txt "%%%%%" " ;ba"
	./a03.out test_1e6.txt b.txt "%aaa%" " ;bd"
	./a03.out test_1e6.txt b.txt "%aca%%cc%" " ;bd"
	./a03.out test_1e6.txt b.txt "%a%c%a%c%a%c%" " ;bd"
	./a04.out test_1e2.txt b.txt "% %" " "
	./a04.out test_1e2.txt b.txt "% %b% %" "; "
	./a04.out test_1e2.txt b.txt "%aa%" "; "
	./a04.out test_1e4.txt b.txt "d %" "; "
	./a04.out test_1e4.txt b.txt "%b% % %c%c" "; ab"
	./a04.out test_1e4.txt b.txt "% % % % % %" "; ab"
	./a04.out test_1e6.txt b.txt "%a% a%%bb% %aaa%" "; cd"
	./a04.out test_1e6.txt b.txt "%% %a %ab%ba% %bbb%" "; cd"
	./a04.out test_1e6.txt b.txt "a%a b%b a%a" "; cd"
	./a04.out test_1e6.txt b.txt "%cdc% %c%c%c%c %d%d%dc%" "; ba"
	./a05.out test_1e2.txt b.txt "a[a-c][a-m]" " ;d"
	./a05.out test_1e2.txt b.txt "[a-b][a-d][c-d]b" " ;"
	./a05.out test_1e2.txt b.txt "[a-b]b[c-c]" " ;d"
	./a05.out test_1e2.txt b.txt "ab[a-n][a-b]b" " ;dec"
	./a05.out test_1e4.txt b.txt "[a-n]" " ;"
	./a05.out test_1e4.txt b.txt "[a-n]ccc[b-c][c-d]" " ;"
	./a05.out test_1e4.txt b.txt "a[a-d]b[c-c]" " ;"
	./a05.out test_1e4.txt b.txt "[c-d]d[c-d]d" " ;ba"
	./a05.out test_1e6.txt b.txt "[c-n][c-d][c-c][c-d]" " ;bea"
	./a05.out test_1e6.txt b.txt "[a-d]ad[--a]" " ;bec"
	./a05.out test_1e6.txt b.txt "[a-d][a-c][a-b][a-a][c-d][b-d][a-d]" " ;"
	./a05.out test_1e6.txt b.txt "[--b]b[--b]" " ;a"
	./a06.out test_1e2.txt b.txt "a[^b-c][^a-b]" " ;d"
	./a06.out test_1e2.txt b.txt "[^a-b]b" " ;"
	./a06.out test_1e2.txt b.txt "[^a-b]b[^c-c]" " ;"
	./a06.out test_1e2.txt b.txt "[^--a][^--b]" " ;"
	./a06.out test_1e4.txt b.txt "[^m-n]" " ;"
	./a06.out test_1e4.txt b.txt "[^a-n]c[^b-c][^c-d]" " ;"
	./a06.out test_1e4.txt b.txt "a[^--b]b[^c-c]" " ;"
	./a06.out test_1e4.txt b.txt "[^c-d]d[^c-d]d" " ;ba"
	./a06.out test_1e6.txt b.txt "[^c-n][^c-d][^c-c][^c-d]" " ;bea"
	./a06.out test_1e6.txt b.txt "[^a-d]ad[^--a]" " ;bec"
	./a06.out test_1e6.txt b.txt "[^a-c][^a-b][^a-a][^c-d][^b-d]" " ;"
	./a06.out test_1e6.txt b.txt "[^--b]b[^--b]" " ;a"
