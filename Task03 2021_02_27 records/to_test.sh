#!/bin/bash
var_1="tests/"
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
./erase_time_and_prefix.sh "$var_1"myres1e2.txt
./erase_time_and_prefix.sh "$var_1"myres1e4.txt
./erase_time_and_prefix.sh "$var_1"myres1e6.txt
echo "erasing end"
diff -w -B "$var_1"myres1e2.txt "$var_1"res1e2_3_2021_03_02.txt
diff -w -B -q -s "$var_1"myres1e2.txt "$var_1"res1e2_3_2021_03_02.txt

diff -w -B "$var_1"myres1e4.txt "$var_1"res1e4_3_2021_03_02.txt
diff -w -B -q -s "$var_1"myres1e4.txt "$var_1"res1e4_3_2021_03_02.txt

diff -w -B "$var_1"myres1e6.txt "$var_1"res1e6_3_2021_03_02.txt
diff -w -B -q -s "$var_1"myres1e6.txt "$var_1"res1e6_3_2021_03_02.txt
echo "end test"
