#!/bin/bash
sed -e '/^\s*$/e' -e 's/Elapsed = [0-9]*.[0-9]*//' -e '/g++/d' -e 's/\s\+/ /' -e '/^[a-zA-Z 0-9]*$/d'  -e 's/^.*\.out //g' res_1_2020_02_20.txt > dimas_summary.txt
echo "start test (my_summary.txt and dimas_summary.txt)"
date +%c
make test > my_summary.txt
date +%c
sed -i -e '/^\s*$/e' -e 's/Elapsed = [0-9]*.[0-9]*//' -e '/g++/d' -e 's/\s\+/ /' -e '/^[a-zA-Z 0-9]*$/d'  -e 's/^.*\.out //g' my_summary.txt
diff -y -i --suppress-common-lines  my_summary.txt dimas_summary.txt
diff -q -s my_summary.txt dimas_summary.txt
echo "end test"
