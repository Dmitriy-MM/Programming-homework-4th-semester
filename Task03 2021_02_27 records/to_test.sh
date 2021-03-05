#!/bin/bash

# Перед запуском теста надо вырезать Elapsed из эталонных файлов: ./erase_elapsed_time.sh res1e2_3_2021_03_02.txt и остальных.
echo "making"
make clean > __tmp.txt
make > __tmp.txt
rm -f __tmp.txt
echo "making end"
echo "start test"
date +%c
make test
date +%c
echo "erasing \"Elapsed = \""
./erase_elapsed_time.sh myres1e2.txt
./erase_elapsed_time.sh myres1e4.txt
./erase_elapsed_time.sh myres1e6.txt
echo "erasing end"
diff -w -B myres1e2.txt res1e2_3_2021_03_02.txt
diff -w -B -q -s myres1e2.txt res1e2_3_2021_03_02.txt

diff -w -B myres1e4.txt res1e4_3_2021_03_02.txt
diff -w -B -q -s myres1e4.txt res1e4_3_2021_03_02.txt

diff -w -B myres1e6.txt res1e6_3_2021_03_02.txt
diff -w -B -q -s myres1e6.txt res1e6_3_2021_03_02.txt
echo "end test"
