#!/bin/bash

GREEN="\e[32m"
RED="\e[31m"
RESET="\e[0m"


g++ -std=c++17 -I ./include testGenerator.cpp ./src/utils.cpp -o testGenerator

mkdir -p output
make clean && make
echo ''
for file in $(ls tests); do
    ./testGenerator tests/$file
    ./orderBook tests/$file $file.dat
    # ./responsereader output/$file.dat > tests/$file/correctOutput/output.txt 
    
    ./responsereader output/$file.dat > temp.txt
    if diff -q temp.txt tests/$file/correctOutput/output.txt > /dev/null; then
        echo -e "${GREEN}$file passed ${RESET}"
    else
        echo -e "${RED}$file failed ${RESET}"
    fi
    rm temp.txt
done
echo ''


make clean
rm testGenerator