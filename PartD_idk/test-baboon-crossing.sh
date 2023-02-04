#!/bin/bash
# script takes 3 arguments that are given to the master worker program

gcc -o baboon_.c -lpthread
./baboon_ $1 $2 $3 > output 
awk -v capacity=$1 -v baboons_left=$2 -v baboons_right=$3 -f check.awk output
# rm output