#!/bin/bash


g++ -O3 -march=native -fopt-info-vec "$1" -o prg 2> vectorized-bb.txt 
g++ -O3 -march=native -fopt-info-vec-missed "$1" -o prg 2> vectorized-missed-bb.txt 
g++ -O3 -march=native -fopt-info-vec-all "$1" -o prg 2> vectorized-all-bb.txt 


