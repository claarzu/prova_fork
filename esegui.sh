#!/bin/bash
clear
cd ~/Scrivania/prova_fork
killall -9 game
make clean
make
./game
