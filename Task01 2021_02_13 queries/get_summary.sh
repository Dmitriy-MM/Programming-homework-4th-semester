#!/bin/bash
#sed -e '/^\s*$/e' -e 's/Elapsed = [0-9]*.[0-9]*//' -e '/g++/d' -e 's/\s\+/ /' -e '/^[a-zA-Z 0-9]*$/d' -e 's/^.*\([/\\]*.*\.out \)/&/g' res_1_2020_02_20.txt > dimas_summary.txt
echo "Start making summary into \"my_summary.txt\":"
date +%c
make test > my_summary.txt
date +%c
sed -i -e '/^\s*$/e' my_summary.txt # Удалить пустые и состоящие из пробельных символов строки.
#sed -i -e 's/Elapsed = [0-9]*.[0-9]*//' my_summary.txt # Удалить из вывода  время работы программы.
sed -i -e '/g++/d' my_summary.txt # Удалить строки команды компиляции программ.
sed -i -e 's/\s\+/ /' my_summary.txt # Заменить повторяющиеся подряд пробельные символы на один пробел.
sed -i -e '/^[a-zA-Z 0-9]*$/d' my_summary.txt # Удалить строки "не имеющие отношения" к запуску программы (a01 etc).
sed -i -e 's/^.*\([/\\]*.*\.out \)/&/g' my_summary.txt
#diff -y -i --suppress-common-lines  my_summary.txt dimas_summary.txt
#diff -q -s my_summary.txt dimas_summary.txt
echo "End, summary maked."

# Памятка по sed https://www.tutorialspoint.com/unix/unix-regular-expressions.htm 
# Обратные ссылки это & и \1, \2,...: "s/.*\(aaaa\).*/&/".
