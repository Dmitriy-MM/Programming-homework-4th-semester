#!/bin/bash
tests_folder="tests/"
shell_folder="shell/"
cur_date="5_2021_03_16"
# Перед запуском теста надо вырезать Elapsed из эталонных файлов: ./erase_elapsed_time.sh res1e2_3_"$cur_date".txt и остальных.
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
#"$shell_folder"./erase_time_and_prefix.sh "$tests_folder"myres1e6.txt

"$shell_folder"./erase_time_and_prefix.sh "$tests_folder"res1e2_"$cur_date".txt
"$shell_folder"./erase_time_and_prefix.sh "$tests_folder"res1e4_"$cur_date".txt
#"$shell_folder"./erase_time_and_prefix.sh "$tests_folder"res1e6_"$cur_date".txt

echo "erasing end"
#diff -w -B  -y "$tests_folder"myres1e2.txt "$tests_folder"res1e2_"$cur_date".txt
diff -w -B -q -s "$tests_folder"myres1e2.txt "$tests_folder"res1e2_"$cur_date".txt

#diff -w -B "$tests_folder"myres1e4.txt "$tests_folder"res1e4_"$cur_date".txt
diff -w -B -q -s "$tests_folder"myres1e4.txt "$tests_folder"res1e4_"$cur_date".txt

#diff -w -B "$tests_folder"myres1e6.txt "$tests_folder"res1e6_"$cur_date".txt
#diff -w -B -q -s "$tests_folder"myres1e6.txt "$tests_folder"res1e6_"$cur_date".txt
echo "end test"
