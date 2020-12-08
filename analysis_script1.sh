#!/bin/sh

export OMP_THREAD_NUM=2

sudo perf stat ./mysort -s bsqu
sudo perf stat ./mysort -s treiberst
sudo perf stat ./mysort -s msqu
sudo perf stat ./mysort -s sglst
sudo perf stat ./mysort -s sglqu
sudo perf stat ./mysort -s sglst -a Elim
sudo perf stat ./mysort -s treiberst -a Elim
sudo perf stat ./mysort -s sglst -a fc
sudo perf stat ./mysort -s sglqu -a fc

export OMP_THREAD_NUM=4

sudo perf stat ./mysort -s bsqu
sudo perf stat ./mysort -s treiberst
sudo perf stat ./mysort -s msqu
sudo perf stat ./mysort -s sglst
sudo perf stat ./mysort -s sglqu
sudo perf stat ./mysort -s sglst -a Elim
sudo perf stat ./mysort -s treiberst -a Elim
sudo perf stat ./mysort -s sglst -a fc
sudo perf stat ./mysort -s sglqu -a fc

export OMP_THREAD_NUM=8

sudo perf stat ./mysort -s bsqu
sudo perf stat ./mysort -s treiberst
sudo perf stat ./mysort -s msqu
sudo perf stat ./mysort -s sglst
sudo perf stat ./mysort -s sglqu
sudo perf stat ./mysort -s sglst -a Elim
sudo perf stat ./mysort -s treiberst -a Elim
sudo perf stat ./mysort -s sglst -a fc
sudo perf stat ./mysort -s sglqu -a fc

export OMP_THREAD_NUM=16

sudo perf stat ./mysort -s bsqu
sudo perf stat ./mysort -s treiberst
sudo perf stat ./mysort -s msqu
sudo perf stat ./mysort -s sglst
sudo perf stat ./mysort -s sglqu
sudo perf stat ./mysort -s sglst -a Elim
sudo perf stat ./mysort -s treiberst -a Elim
sudo perf stat ./mysort -s sglst -a fc
sudo perf stat ./mysort -s sglqu -a fc

export OMP_THREAD_NUM=32

sudo perf stat ./mysort -s bsqu
sudo perf stat ./mysort -s treiberst
sudo perf stat ./mysort -s msqu
sudo perf stat ./mysort -s sglst
sudo perf stat ./mysort -s sglqu
sudo perf stat ./mysort -s sglst -a Elim
sudo perf stat ./mysort -s treiberst -a Elim
sudo perf stat ./mysort -s sglst -a fc
sudo perf stat ./mysort -s sglqu -a fc

export OMP_THREAD_NUM=64

sudo perf stat ./mysort -s bsqu
sudo perf stat ./mysort -s treiberst
sudo perf stat ./mysort -s msqu
sudo perf stat ./mysort -s sglst
sudo perf stat ./mysort -s sglqu
sudo perf stat ./mysort -s sglst -a Elim
sudo perf stat ./mysort -s treiberst -a Elim
sudo perf stat ./mysort -s sglst -a fc
sudo perf stat ./mysort -s sglqu -a fc

