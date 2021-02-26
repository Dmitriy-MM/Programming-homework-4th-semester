#!/bin/bash
cat $1 > dimas_summary.txt
sed -i -e '/^\s*$/e' -e 's/Elapsed = [0-9]*.[0-9]*//' -e '/g++/d' -e 's/\s\+/ /' -e '/^[a-zA-Z 0-9]*$/d'  -e 's/^.*\(a[0-9]\+\.out\)/\1/g' dimas_summary.txt
sed -i -e '/^\s*$/e' -e 's/Elapsed = [0-9]*.[0-9]*//' -e '/g++/d' -e 's/\s\+/ /' -e '/^[a-zA-Z 0-9]*$/d'  -e 's/^.*\(a[0-9]\+\.out\)/\1/g' my_summary.txt
diff -q -s my_summary.txt dimas_summary.txt
diff -y my_summary.txt dimas_summary.txt
diff -q -s my_summary.txt dimas_summary.txt

