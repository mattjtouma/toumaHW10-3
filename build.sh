#!/bin/bash

# Echo a message
echo "Compiling..."

# compile the program
g++ myLed.cpp -o myLed.cgi -lcgicc

# Echo a message
echo "done"
