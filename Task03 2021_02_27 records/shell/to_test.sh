#!/bin/bash
tests_folder="tests/"
shell_folder="shell/"
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
"$shell_folder"./erase_time_and_prefix.sh "$tests_folder"myres1e2.txt
"$shell_folder"./erase_time_and_prefix.sh "$tests_folder"myres1e4.txt
"$shell_folder"./erase_time_and_prefix.sh "$tests_folder"myres1e6.txt
echo "erasing end"
diff -w -B "$tests_folder"myres1e2.txt "$tests_folder"res1e2_3_2021_03_02.txt
diff -w -B -q -s "$tests_folder"myres1e2.txt "$tests_folder"res1e2_3_2021_03_02.txt

diff -w -B "$tests_folder"myres1e4.txt "$tests_folder"res1e4_3_2021_03_02.txt
diff -w -B -q -s "$tests_folder"myres1e4.txt "$tests_folder"res1e4_3_2021_03_02.txt

diff -w -B "$tests_folder"myres1e6.txt "$tests_folder"res1e6_3_2021_03_02.txt
diff -w -B -q -s "$tests_folder"myres1e6.txt "$tests_folder"res1e6_3_2021_03_02.txt
echo "end test"
