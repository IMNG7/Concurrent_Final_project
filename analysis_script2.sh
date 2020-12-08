#!/bin/sh

export OMP_THREAD_NUM=16

sudo perf stat ./mysort -s bsqu -i 10000
sudo perf stat ./mysort -s treiberst -i 10000
sudo perf stat ./mysort -s msqu -i 10000
sudo perf stat ./mysort -s sglst -i 10000
sudo perf stat ./mysort -s sglqu -i 10000
sudo perf stat ./mysort -s sglst -a Elim -i 10000
sudo perf stat ./mysort -s treiberst -a Elim -i 10000
sudo perf stat ./mysort -s sglst -a fc -i 10000
sudo perf stat ./mysort -s sglqu -a fc -i 10000


sudo perf stat ./mysort -s bsqu -i 20000
sudo perf stat ./mysort -s treiberst -i 20000
sudo perf stat ./mysort -s msqu -i 20000
sudo perf stat ./mysort -s sglst -i 20000
sudo perf stat ./mysort -s sglqu -i 20000
sudo perf stat ./mysort -s sglst -a Elim -i 20000
sudo perf stat ./mysort -s treiberst -a Elim -i 20000
sudo perf stat ./mysort -s sglst -a fc -i 20000
sudo perf stat ./mysort -s sglqu -a fc -i 20000

sudo perf stat ./mysort -s bsqu -i 30000
sudo perf stat ./mysort -s treiberst -i 30000
sudo perf stat ./mysort -s msqu -i 30000
sudo perf stat ./mysort -s sglst -i 30000
sudo perf stat ./mysort -s sglqu -i 30000
sudo perf stat ./mysort -s sglst -a Elim -i 30000
sudo perf stat ./mysort -s treiberst -a Elim -i 30000
sudo perf stat ./mysort -s sglst -a fc -i 30000
sudo perf stat ./mysort -s sglqu -a fc -i 30000


sudo perf stat ./mysort -s bsqu -i 40000
sudo perf stat ./mysort -s treiberst -i 40000
sudo perf stat ./mysort -s msqu -i 40000
sudo perf stat ./mysort -s sglst -i 40000
sudo perf stat ./mysort -s sglqu -i 40000
sudo perf stat ./mysort -s sglst -a Elim -i 40000
sudo perf stat ./mysort -s treiberst -a Elim -i 40000
sudo perf stat ./mysort -s sglst -a fc -i 40000
sudo perf stat ./mysort -s sglqu -a fc -i 40000


sudo perf stat ./mysort -s bsqu -i 50000
sudo perf stat ./mysort -s treiberst -i 50000
sudo perf stat ./mysort -s msqu -i 50000
sudo perf stat ./mysort -s sglst -i 50000
sudo perf stat ./mysort -s sglqu -i 50000
sudo perf stat ./mysort -s sglst -a Elim -i 50000
sudo perf stat ./mysort -s treiberst -a Elim -i 50000
sudo perf stat ./mysort -s sglst -a fc -i 50000
sudo perf stat ./mysort -s sglqu -a fc -i 50000

sudo perf stat ./mysort -s bsqu -i 60000
sudo perf stat ./mysort -s treiberst -i 60000
sudo perf stat ./mysort -s msqu -i 60000
sudo perf stat ./mysort -s sglst -i 60000
sudo perf stat ./mysort -s sglqu -i 60000
sudo perf stat ./mysort -s sglst -a Elim -i 60000
sudo perf stat ./mysort -s treiberst -a Elim -i 60000
sudo perf stat ./mysort -s sglst -a fc -i 60000
sudo perf stat ./mysort -s sglqu -a fc -i 60000

