#!/bin/bash

function build {
    mkdir build
    cd build
    cmake ..
    cmake --build . -j
    cd ..
}

function run {
    cd build
    ./psql_server &
}

FILE_EXE=$(pwd)/build/psql_server

if ! [ -f "$FILE_EXE" ]
then
    build
fi

fuser -k 4000/tcp

if [ -f "$FILE_EXE" ]
then
    run
fi