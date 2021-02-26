#!/bin/bash
if [ -f "$1" ]
then
	echo "making"
	make clean > __tmp.txt
	make > __tmp.txt
	rm -f __tmp.txt
	echo "making end"
	#cat $1 > dimas_summary.txt
	#sed -i -e '/^\s*$/e' -e 's/Elapsed = [0-9]*.[0-9]*//' -e '/g++/d' -e 's/\s\+/ /' -e '/^[a-zA-Z 0-9]*$/d'  -e 's/^.*\([/\\]*.*\.out \)/&/g' dimas_summary.txt
	echo "start test (my_summary.txt and dimas_summary.txt)"
	date +%c
	make test > my_summary.txt
	date +%c
	sed -i -e '/^\s*$/e' -e 's/Elapsed = [0-9]*.[0-9]*//' -e '/g++/d' -e 's/\s\+/ /' -e '/^[a-zA-Z 0-9]*$/d'  -e 's/^.*\([/\\]*.*\.out \)/&/g' my_summary.txt
	./to_check_only.sh $1
	echo "end test"
else
	echo "Usage: $0 results_filename.txt"
	exit [1]
fi
